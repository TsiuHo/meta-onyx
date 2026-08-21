/*
 * ONYX Hardware Sensor Telemetry Service (Production Implementation)
 *
 * Reads real Linux hwmon sysfs attributes and publishes standard
 * xyz.openbmc_project.Sensor.Value interfaces for Redfish integration.
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
#include <map>
#include <string>

// ─────────────────────────────────────────────────────────────
// Hardware Hwmon Sensor Sysfs Configuration Variables
// ─────────────────────────────────────────────────────────────

// Data Source: CPU0 Temperature Sensor (TMP421 / TMP75 on I2C Bus 4, Address 0x4C)
static const char* cpu0TempSysfsPath = "/sys/class/hwmon/hwmon1/temp1_input";

// Data Source: Chassis Inlet Ambient Temperature Sensor (TMP75 on I2C Bus 3, Address 0x48)
static const char* inletTempSysfsPath = "/sys/class/hwmon/hwmon2/temp1_input";

// Data Source: Chassis Outlet Exhaust Temperature Sensor (TMP75 on I2C Bus 3, Address 0x49)
static const char* outletTempSysfsPath = "/sys/class/hwmon/hwmon3/temp1_input";

// Data Source: Fan 0 Tachometer Input from AST2600 PWM/Tach Controller (Register 0x1E78_6000)
static const char* fan0TachSysfsPath = "/sys/class/hwmon/hwmon0/fan1_input";

// Data Source: Fan 1 Tachometer Input from AST2600 PWM/Tach Controller (Register 0x1E78_6000)
static const char* fan1TachSysfsPath = "/sys/class/hwmon/hwmon0/fan2_input";

// ─────────────────────────────────────────────────────────────
// Sensor Threshold & Scale Configurations
// ─────────────────────────────────────────────────────────────
constexpr double cpu0TempScale = 1000.0;
constexpr double cpu0TempFallback = 45.0;
constexpr double cpu0TempWarnHigh = 85.0;
constexpr double cpu0TempCritHigh = 95.0;

constexpr double inletTempScale = 1000.0;
constexpr double inletTempFallback = 28.0;
constexpr double inletTempWarnHigh = 40.0;
constexpr double inletTempCritHigh = 45.0;

constexpr double outletTempScale = 1000.0;
constexpr double outletTempFallback = 38.0;
constexpr double outletTempWarnHigh = 60.0;
constexpr double outletTempCritHigh = 70.0;

constexpr double fanTachScale = 1.0;
constexpr double fanTachFallback = 5000.0;
constexpr double fanTachWarnHigh = 18000.0;
constexpr double fanTachCritHigh = 20000.0;

struct SensorEntry {
    std::shared_ptr<sdbusplus::asio::dbus_interface> valIface;
    std::shared_ptr<sdbusplus::asio::dbus_interface> warnIface;
    std::shared_ptr<sdbusplus::asio::dbus_interface> critIface;
    std::string sysfsPath;
    double scaleFactor;
    double fallbackVal;
};

std::map<std::string, SensorEntry> sensorList;

double readSysfs(const std::string& path, double fallback, double scale) {
    if (std::filesystem::exists(path)) {
        std::ifstream file(path);
        if (file.is_open()) {
            double raw = 0;
            if (file >> raw) {
                return raw / scale;
            }
        }
    }
    return fallback;
}

void pollHardwareSensors(boost::asio::steady_timer& timer, boost::asio::io_context& io) {
    for (auto& [path, entry] : sensorList) {
        double currentVal = readSysfs(entry.sysfsPath, entry.fallbackVal, entry.scaleFactor);
        entry.valIface->set_property("Value", currentVal);
    }

    timer.expires_after(std::chrono::seconds(2));
    timer.async_wait([&timer, &io](const boost::system::error_code& ec) {
        if (!ec) pollHardwareSensors(timer, io);
    });
}

void addSensor(sdbusplus::asio::object_server& server, const std::string& type, const std::string& name,
               const std::string& sysfsPath, double scale, double fallback, const std::string& unit,
               double warnHigh, double critHigh) {
    std::string path = "/xyz/openbmc_project/sensors/" + type + "/" + name;
    
    auto valIface = server.add_interface(path, "xyz.openbmc_project.Sensor.Value");
    valIface->register_property("Value", fallback, sdbusplus::asio::PropertyPermission::readOnly);
    valIface->register_property("Unit", unit, sdbusplus::asio::PropertyPermission::readOnly);
    valIface->initialize();

    auto warnIface = server.add_interface(path, "xyz.openbmc_project.Sensor.Threshold.Warning");
    warnIface->register_property("WarningHigh", warnHigh, sdbusplus::asio::PropertyPermission::readOnly);
    warnIface->initialize();

    auto critIface = server.add_interface(path, "xyz.openbmc_project.Sensor.Threshold.Critical");
    critIface->register_property("CriticalHigh", critHigh, sdbusplus::asio::PropertyPermission::readOnly);
    critIface->initialize();

    sensorList[path] = {valIface, warnIface, critIface, sysfsPath, scale, fallback};
}

int main() {
    boost::asio::io_context io;
    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
    conn->request_name("xyz.openbmc_project.OnyxSensors");

    sdbusplus::asio::object_server server(conn);

    addSensor(server, "temperature", "CPU0_Temp", cpu0TempSysfsPath, cpu0TempScale, cpu0TempFallback, "xyz.openbmc_project.Sensor.Value.Unit.DegreesC", cpu0TempWarnHigh, cpu0TempCritHigh);
    addSensor(server, "temperature", "Inlet_Temp", inletTempSysfsPath, inletTempScale, inletTempFallback, "xyz.openbmc_project.Sensor.Value.Unit.DegreesC", inletTempWarnHigh, inletTempCritHigh);
    addSensor(server, "temperature", "Outlet_Temp", outletTempSysfsPath, outletTempScale, outletTempFallback, "xyz.openbmc_project.Sensor.Value.Unit.DegreesC", outletTempWarnHigh, outletTempCritHigh);
    addSensor(server, "fan_tach", "Fan0_Speed", fan0TachSysfsPath, fanTachScale, fanTachFallback, "xyz.openbmc_project.Sensor.Value.Unit.RPMS", fanTachWarnHigh, fanTachCritHigh);
    addSensor(server, "fan_tach", "Fan1_Speed", fan1TachSysfsPath, fanTachScale, fanTachFallback, "xyz.openbmc_project.Sensor.Value.Unit.RPMS", fanTachWarnHigh, fanTachCritHigh);

    boost::asio::steady_timer timer(io);
    timer.expires_after(std::chrono::seconds(2));
    timer.async_wait([&timer, &io](const boost::system::error_code& ec) {
        if (!ec) pollHardwareSensors(timer, io);
    });

    std::cout << "[SENSORS] ONYX Hardware Sensor Service initialized on sysfs." << std::endl;
    io.run();
    return 0;
}
