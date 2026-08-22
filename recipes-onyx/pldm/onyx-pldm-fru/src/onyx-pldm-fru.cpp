#include <iostream>
#include <sdbusplus/bus.hpp>
#include <phosphor-logging/log.hpp>

using namespace phosphor::logging;

/* 
 * Practical hardware variable declaration area
 * After getting the actual hardware data, please fill the following variables with the correct values.
 */
constexpr uint16_t HGX_FRU_RECORD_ID = 0xFFFF; // TODO: HGX Baseboard FRU Record Set Identifier

void execute_task()
{
    log<level::INFO>("Obtaining HGX Baseboard PLDM FRU data...");
    // Implementation highlights:
    // 1. Send PLDM GetFRURecordTableMetadata to get length and structure
    // 2. Send GetFRURecordTable to get complete FRU binary data
    // 3. Parse out the Serial Number (SN), Part Number (PN), and Manufacturer according to the PLDM Type 4 specification
    // 4. Call the DBus interface of phosphor-inventory-manager with the parsed results to file them
}

int main()
{
    log<level::INFO>("Starting PLDM Type 4 HGX Baseboard FRU Data service");
    
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
