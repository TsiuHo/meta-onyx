#include <iostream>
#include <sdbusplus/bus.hpp>
#include <phosphor-logging/log.hpp>

using namespace phosphor::logging;

/* 
 * Practical hardware variable declaration area
 * After getting the actual HGX / hardware schematics, please fill the following variables with the correct values.
 */
// TODO: Fill in the I2C/SMBus Bus Number from the hardware design (the Bus connected to the HGX Baseboard)
constexpr int HGX_SMBUS_BUS_NUM = -1; 

// TODO: Fill in the start and end I2C addresses of GPU and NVSwitch on the SMBus
constexpr uint8_t GPU_I2C_ADDR_START = 0x00; 
constexpr uint8_t GPU_I2C_ADDR_END   = 0x00; 

// TODO: Default starting MCTP Endpoint ID (EID) allocated to the GPU
constexpr uint8_t GPU_MCTP_EID_START = 10;

/**
 * @brief Scan SMBus and setup MCTP Endpoint
 * 
 * This function is responsible for calling mctpd (MCTP Daemon) via DBus,
 * Register the scanned I2C addresses as MCTP Endpoints.
 */
void discoverAndRouteSMBus()
{
    if (HGX_SMBUS_BUS_NUM == -1) {
        log<level::ERR>("HGX_SMBUS_BUS_NUM is not set yet, cannot perform hardware scanning!");
        return;
    }

    log<level::INFO>("Starting to scan HGX SMBus to establish MCTP connection...");
    // Implementation details:
    // 1. Scan HGX_SMBUS_BUS_NUM via i2c-dev or DBus
    // 2. For devices that respond in the [GPU_I2C_ADDR_START, GPU_I2C_ADDR_END] range
    // 3. Send a SetupEndpoint DBus call to xyz.openbmc_project.MCTP
}

int main()
{
    log<level::INFO>("Starting ONYX MCTP over SMBus Discovery service");
    
    try {
        auto bus = sdbusplus::bus::new_default();
        
        discoverAndRouteSMBus();

        // Enter the event loop, continuously monitor plug or hot-plug events
        bus.process_loop();
    } catch (const std::exception& e) {
        log<level::ERR>("Service exception occurred", entry("ERROR=%s", e.what()));
        return -1;
    }

    return 0;
}
