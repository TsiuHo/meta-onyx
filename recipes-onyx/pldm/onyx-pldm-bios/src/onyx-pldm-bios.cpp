/*
 * ONYX PLDM BIOS Configuration Manager (Production Hardware Implementation)
 *
 * Implements direct D-Bus communication with upstream OpenBMC PLDM daemon (pldmd)
 * and DMTF DSP0247 BIOS Control and Configuration interface.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sdbusplus/bus.hpp>
#include <sdbusplus/message.hpp>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

// ─────────────────────────────────────────────────────────────
// Hardware & D-Bus Platform Configuration Variables
// ─────────────────────────────────────────────────────────────

// Data Source: Upstream OpenBMC PLDM daemon service name managing BIOS attributes
static const char* pldmDbusService = "xyz.openbmc_project.PLDM";

// Data Source: Upstream OpenBMC BIOS configuration object path
static const char* pldmBiosConfigPath = "/xyz/openbmc_project/bios_config/manager";

// Data Source: Upstream BIOS Config Manager interface (DMTF DSP0247 mapping)
static const char* pldmBiosConfigIface = "xyz.openbmc_project.BIOSConfig.Manager";

// Data Source: AST2600 LPC KCS Channel 3 character device (x86 Host BIOS to BMC MCTP bus)
// Hardware Mapping: AST2600 KCS3 Register base 0x1E78_9024 (DTS node: kcs3: kcs@1e789024)
static const char* kcsMctpDevNode = "/dev/aspeed-kcs3";

// Data Source: Host BIOS MCTP Endpoint ID (EID) assigned in MCTP routing table
constexpr uint8_t hostBiosMctpEid = 9;

// Data Source: DMTF PLDM Message Type 0x03 (BIOS Control and Configuration)
constexpr uint8_t pldmTypeBios = 0x03;

// Data Source: Host BIOS default Boot Order string
static const char* defaultBootOrder = "PXE,HDD,USB";

// Data Source: Host BIOS default Secure Boot state
static const char* defaultSecureBoot = "Disabled";

void setBiosAttribute(sdbusplus::bus_t& bus, const std::string& attrName, const std::string& attrValue) {
    try {
        auto method = bus.new_method_call(
            pldmDbusService,
            pldmBiosConfigPath,
            pldmBiosConfigIface,
            "SetAttribute"
        );

        using AttributeValue = std::variant<int64_t, std::string>;
        AttributeValue val = attrValue;

        method.append(attrName, val);
        auto reply = bus.call(method);

        std::cout << "[BIOS-CFG] Successfully updated attribute '" << attrName 
                  << "' to value '" << attrValue << "' via pldmd D-Bus." << std::endl;
    }
    catch (const sdbusplus::exception_t& e) {
        std::cerr << "[BIOS-CFG-ERROR] Failed to set attribute: " << e.what() << std::endl;
        std::cerr << "[INFO] When running on target motherboard, ensure 'pldmd' is active." << std::endl;
    }
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "ONYX PLDM BIOS Configuration Manager (Production Driver)\n";
    std::cout << "KCS Device: " << kcsMctpDevNode << ", Host EID: " << (int)hostBiosMctpEid << "\n";
    std::cout << "========================================================\n";

    auto bus = sdbusplus::bus::new_default();

    setBiosAttribute(bus, "BootOrder", defaultBootOrder);
    setBiosAttribute(bus, "SecureBoot", defaultSecureBoot);

    return 0;
}
