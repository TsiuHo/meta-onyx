#include <iostream>
#include <sdbusplus/bus.hpp>
#include <phosphor-logging/log.hpp>

using namespace phosphor::logging;

/* 
 * Practical hardware variable declaration area
 * After getting the actual hardware data, please fill the following variables with the correct values.
 */
constexpr uint8_t SPDM_MEASURE_BLOCK = 0xFF; // TODO: Index of the measurement block to be read

void execute_task()
{
    log<level::INFO>("Reading device SPDM firmware measurements...");
    // Implementation highlights:
    // 1. After successful authentication, send the GET_MEASUREMENTS command
    // 2. The device returns the Hash value (like SHA-384) of firmware layers (like Bootloader, Main FW)
    // 3. The BMC compares the obtained Measurements with the factory whitelist (Golden Hash)
    // 4. If comparison fails, trigger a Security Event and potentially cut off power to the device (Hardware Root of Trust PRoT)
}

int main()
{
    log<level::INFO>("Starting SPDM Attestation & Measurement service");
    
    try {
        auto bus = sdbusplus::bus::new_default();
        execute_task();
        bus.process_loop();
    } catch (const std::exception& e) {
        log<level::ERR>("Service exception occurred", entry("ERROR=%s", e.what()));
        return -1;
    }
    return 0;
}
