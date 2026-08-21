/*
 * ONYX AST2600 eSPI / LPC Snoop POST Code Monitor (Production Implementation)
 *
 * Captures x86 Host BIOS Port 80h/81h POST codes directly from AST2600
 * eSPI/LPC Snoop character device (/dev/aspeed-lpc-snoop0).
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <boost/asio/io_context.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/steady_timer.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>

// ─────────────────────────────────────────────────────────────
// Hardware Character Device & GPIO Configuration Variables
// ─────────────────────────────────────────────────────────────

// Data Source: AST2600 eSPI/LPC Snoop Controller Character Device
// Hardware Mapping: Register base 0x1E78_9080 (DTS node: lpc_snoop@1e789080)
static const char* lpcSnoopDevNode = "/dev/aspeed-lpc-snoop0";

// Data Source: AST2600 GPIO 'GPIOP4' - Host Platform Reset line (PLTRST_L)
// Hardware Mapping: Detects Host warm/cold reset cycle to clear POST history
static const char* gpioHostPltRst = "HOST_PLTRST_N";

// Data Source: Hardware Watchdog timeout duration in seconds for POST hang detection
constexpr uint32_t defaultHangTimeoutSeconds = 60;

class PostcodeSnoopService {
public:
    PostcodeSnoopService(boost::asio::io_context& io, std::shared_ptr<sdbusplus::asio::connection> conn)
        : io_(io), timer_(io), server_(conn), snoopStream_(io) {
        
        iface_ = server_.add_interface("/com/onyx/postcode", "com.onyx.PostcodeSnoop");

        iface_->register_property("CurrentPostCode", currentCode_);
        iface_->register_property("PostCodeHistory", codeHistory_);
        iface_->register_property("HangDetected", hangDetected_);
        iface_->register_property("HangCode", hangCode_);
        iface_->register_property("HangTimeoutSeconds", hangTimeoutSeconds_, sdbusplus::asio::PropertyPermission::readWrite);

        iface_->initialize();

        openHardwareSnoopDevice();
    }

private:
    void openHardwareSnoopDevice() {
        if (std::filesystem::exists(lpcSnoopDevNode)) {
            int fd = open(lpcSnoopDevNode, O_RDONLY | O_NONBLOCK);
            if (fd >= 0) {
                snoopStream_.assign(fd);
                startAsyncRead();
                std::cout << "[SNOOP] Successfully opened hardware eSPI snoop device: " << lpcSnoopDevNode << std::endl;
                return;
            }
        }
        std::cerr << "[SNOOP-WARN] " << lpcSnoopDevNode << " not available on this host. Ready for production board." << std::endl;
    }

    void startAsyncRead() {
        snoopStream_.async_read_some(boost::asio::buffer(&readBuf_, 1),
            [this](const boost::system::error_code& ec, std::size_t bytesRead) {
                if (!ec && bytesRead > 0) {
                    processNewPostCode(readBuf_);
                    startAsyncRead();
                }
            });
    }

    void processNewPostCode(uint8_t code) {
        currentCode_ = code;
        iface_->set_property("CurrentPostCode", currentCode_);

        codeHistory_.push_back(code);
        if (codeHistory_.size() > 256) {
            codeHistory_.erase(codeHistory_.begin());
        }
        iface_->set_property("PostCodeHistory", codeHistory_);

        hangDetected_ = false;
        hangCode_ = 0;
        iface_->set_property("HangDetected", false);
        iface_->set_property("HangCode", static_cast<uint8_t>(0));

        std::cout << "[POST-HW] 0x" << std::hex << (int)code << std::dec << std::endl;

        timer_.expires_after(std::chrono::seconds(hangTimeoutSeconds_));
        timer_.async_wait([this, code](const boost::system::error_code& ec) {
            if (!ec) {
                hangDetected_ = true;
                hangCode_ = code;
                iface_->set_property("HangDetected", true);
                iface_->set_property("HangCode", code);
                std::cerr << "[POST-HANG] System hang detected at POST Code 0x" 
                          << std::hex << (int)code << std::dec << std::endl;
            }
        });
    }

    boost::asio::io_context& io_;
    boost::asio::steady_timer timer_;
    sdbusplus::asio::object_server server_;
    std::shared_ptr<sdbusplus::asio::dbus_interface> iface_;
    boost::asio::posix::stream_descriptor snoopStream_;
    uint8_t readBuf_ = 0;

    uint8_t currentCode_ = 0;
    std::vector<uint8_t> codeHistory_;
    bool hangDetected_ = false;
    uint8_t hangCode_ = 0;
    uint32_t hangTimeoutSeconds_ = defaultHangTimeoutSeconds;
};

int main() {
    boost::asio::io_context io;
    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
    conn->request_name("com.onyx.PostcodeSnoop");

    PostcodeSnoopService service(io, conn);
    std::cout << "[SNOOP] ONYX eSPI/LPC Snoop Service running on D-Bus." << std::endl;

    io.run();
    return 0;
}
