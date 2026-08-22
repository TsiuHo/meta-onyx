#include <iostream>
#include <sdbusplus/bus.hpp>
#include <phosphor-logging/log.hpp>

using namespace phosphor::logging;

/* 
 * Practical hardware variable declaration area
 * After getting the actual hardware data, please fill the following variables with the correct values.
 */
constexpr uint16_t GPU_TERM_SENSOR_ID = 0xFFFF; // TODO: GPU temperature PDR ID
constexpr uint16_t GPU_POWER_EFFECTER_ID = 0xFFFF; // TODO: GPU power cap Effecter ID

void execute_task()
{
    log<level::INFO>("Sending PLDM GetSensorReading command to GPU...");
    // Implementation highlights:
    // 1. Get Sensor ID based on PDR (Platform Descriptor Record)
    // 2. Construct a PLDM Type 2 GetSensorReading Request packet and send it via MCTP
    // 3. Parse the Response to get the GPU temperature
    // 4. If the temperature is too high, build a SetStateEffecterStates command to lower the power cap (Power Capping)
}

int main()
{
    log<level::INFO>("Starting PLDM Type 2 GPU/NVSwitch Thermal & Power service");
    
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
