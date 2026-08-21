/*
 * ONYX D-Bus OEM Information Service
 *
 * This daemon publishes custom OEM properties on D-Bus so that bmcweb
 * (Redfish) can query and expose them through the Redfish API.
 *
 * D-Bus Service:   com.onyx.OemInfo
 * D-Bus Object:    /com/onyx/oem_info
 * D-Bus Interface: com.onyx.OemInfo
 *
 * Properties:
 *   - Version    (string) : ONYX firmware version
 *   - BoardName  (string) : Board name identifier
 *   - BiosVersion(string) : BIOS version
 *   - Status     (string) : Overall status
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/bus.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <iostream>
#include <string>

int main()
{
    // Create the main event loop
    boost::asio::io_context io;

    // Connect to the system D-Bus
    auto conn = std::make_shared<sdbusplus::asio::connection>(io);

    // Request a well-known D-Bus service name
    conn->request_name("com.onyx.OemInfo");

    // Create an object server to host our D-Bus objects
    sdbusplus::asio::object_server objServer(conn);

    // Create the D-Bus interface on the specified object path
    auto iface = objServer.add_interface(
        "/com/onyx/oem_info",   // Object path
        "com.onyx.OemInfo"      // Interface name
    );

    // ─────────────────────────────────────────────────
    // Register properties on the interface
    // ─────────────────────────────────────────────────

    // Version: the ONYX OEM version string
    iface->register_property("Version", std::string("1.0.1"));

    // BoardName: identifies the hardware platform
    iface->register_property("BoardName", std::string("ONYX-AST2600-EVB"));

    // BiosVersion: BIOS/UEFI version (N/A for EVB)
    iface->register_property("BiosVersion", std::string("N/A"));

    // Status: overall health status
    iface->register_property("Status", std::string("OK"));

    // Finalize and publish the interface on D-Bus
    iface->initialize();

    std::cout << "ONYX OemInfo service started on D-Bus" << std::endl;

    // Set up signal handling for clean shutdown
    boost::asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait(
        [&io](const boost::system::error_code& /*ec*/, int /*signo*/) {
            io.stop();
        });

    // Run the event loop (blocks until SIGINT/SIGTERM)
    io.run();

    return 0;
}
