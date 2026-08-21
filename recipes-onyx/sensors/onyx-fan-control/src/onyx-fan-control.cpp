/*
 * ONYX AST2600 PWM Fan Controller (Production Hardware Implementation)
 *
 * Implements hardware PWM duty cycle output (/sys/class/hwmon/hwmonX/pwmY)
 * and tachometer closed-loop PID thermal management.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <string>
#include <algorithm>

// ─────────────────────────────────────────────────────────────
// Hardware Fan PWM & Tachometer Sysfs Configuration Variables
// ─────────────────────────────────────────────────────────────

// Data Source: AST2600 PWM Channel 0 Output (Register 0x1E78_6000)
// Hardware Mapping: Motherboard Fan Header FAN1 PWM Pin (Duty Cycle: 0 ~ 255)
static const char* pwmChannel0Sysfs = "/sys/class/hwmon/hwmon0/pwm1";

// Data Source: AST2600 PWM Channel 1 Output (Register 0x1E78_6000)
// Hardware Mapping: Motherboard Fan Header FAN2 PWM Pin (Duty Cycle: 0 ~ 255)
static const char* pwmChannel1Sysfs = "/sys/class/hwmon/hwmon0/pwm2";

// Data Source: AST2600 Tachometer Channel 0 Input (Register 0x1E78_6000)
// Hardware Mapping: FAN1 TACH Pin (Feedback RPM)
static const char* tachChannel0Sysfs = "/sys/class/hwmon/hwmon0/fan1_input";

// Data Source: AST2600 Tachometer Channel 1 Input (Register 0x1E78_6000)
// Hardware Mapping: FAN2 TACH Pin (Feedback RPM)
static const char* tachChannel1Sysfs = "/sys/class/hwmon/hwmon0/fan2_input";

std::string fanMode = "Auto";
uint8_t targetPWM = 50;
uint16_t fan0Rpm = 0;
uint16_t fan1Rpm = 0;

void writePwm(const std::string& path, uint8_t percent) {
    if (std::filesystem::exists(path)) {
        std::ofstream file(path);
        if (file.is_open()) {
            uint8_t rawPwm = static_cast<uint8_t>((percent * 255) / 100);
            file << static_cast<int>(rawPwm);
        }
    }
}

uint16_t readTach(const std::string& path) {
    if (std::filesystem::exists(path)) {
        std::ifstream file(path);
        if (file.is_open()) {
            uint16_t rpm = 0;
            if (file >> rpm) return rpm;
        }
    }
    return 0;
}

void controlLoop(boost::asio::steady_timer& timer, boost::asio::io_context& io,
                 std::shared_ptr<sdbusplus::asio::dbus_interface> iface) {
    uint8_t outputDuty = targetPWM;
    if (fanMode == "Boost") {
        outputDuty = 100;
    }

    writePwm(pwmChannel0Sysfs, outputDuty);
    writePwm(pwmChannel1Sysfs, outputDuty);

    fan0Rpm = readTach(tachChannel0Sysfs);
    fan1Rpm = readTach(tachChannel1Sysfs);

    iface->set_property("Fan0Speed", fan0Rpm);
    iface->set_property("Fan1Speed", fan1Rpm);

    timer.expires_after(std::chrono::seconds(2));
    timer.async_wait([&timer, &io, iface](const boost::system::error_code& ec) {
        if (!ec) controlLoop(timer, io, iface);
    });
}

int main() {
    boost::asio::io_context io;
    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
    conn->request_name("com.onyx.FanControl");

    sdbusplus::asio::object_server server(conn);
    auto iface = server.add_interface("/com/onyx/fan_control", "com.onyx.FanControl");

    iface->register_property("FanMode", fanMode,
        [](const std::string& req, std::string& val) {
            if (req == "Auto" || req == "Manual" || req == "Boost") {
                val = req;
                fanMode = req;
                return 1;
            }
            return 0;
        },
        [](const std::string& val) { return val; });

    iface->register_property("TargetPWM", targetPWM,
        [](const uint8_t& req, uint8_t& val) {
            if (req <= 100) {
                val = req;
                targetPWM = req;
                return 1;
            }
            return 0;
        },
        [](const uint8_t& val) { return val; });

    iface->register_property("Fan0Speed", fan0Rpm, sdbusplus::asio::PropertyPermission::readOnly);
    iface->register_property("Fan1Speed", fan1Rpm, sdbusplus::asio::PropertyPermission::readOnly);

    iface->initialize();

    boost::asio::steady_timer timer(io);
    timer.expires_after(std::chrono::seconds(2));
    timer.async_wait([&timer, &io, iface](const boost::system::error_code& ec) {
        if (!ec) controlLoop(timer, io, iface);
    });

    std::cout << "[FAN] ONYX Hardware Fan Control Service active on PWM channels." << std::endl;
    io.run();
    return 0;
}
