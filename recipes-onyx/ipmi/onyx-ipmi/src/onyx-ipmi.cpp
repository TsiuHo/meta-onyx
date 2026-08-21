/*
 * ONYX-IPMI: Custom OEM IPMI Command Provider for AST2600 EVB
 *
 * This file registers several OEM IPMI commands under NetFn 0x30 (OEM).
 * It demonstrates:
 *   1. A simple no-arg command (Get Version)
 *   2. A command with input parameters and output results (Add/Sub)
 *   3. A command with variable-length input/output (Echo)
 *
 * These commands are loaded as a shared library (.so) by the ipmid daemon
 * from /usr/lib/ipmid-providers/ at startup.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ipmid/api.hpp>
#include <phosphor-logging/log.hpp>

#include <cstdint>
#include <string>
#include <vector>

using phosphor::logging::log;
using phosphor::logging::level;

// ─────────────────────────────────────────────────────────────
// OEM NetFn & Command Code Definitions
// ─────────────────────────────────────────────────────────────
// OEM NetFn range: 0x30 ~ 0x3F (usable for vendor-specific commands)
constexpr ipmi::NetFn netFnOnyxOem = 0x30;

// Command codes under NetFn 0x30
constexpr ipmi::Cmd cmdGetOnyxVersion = 0x01;
constexpr ipmi::Cmd cmdOnyxAddSub    = 0x02;
constexpr ipmi::Cmd cmdOnyxEcho      = 0x03;

// ─────────────────────────────────────────────────────────────
// Command 0x01: Get ONYX Version
// ─────────────────────────────────────────────────────────────
// Request:  (none)
// Response: [major(1B)] [minor(1B)] [patch(1B)]
//
// Example:
//   ipmitool raw 0x30 0x01
//   Expected response: 01 00 01  (Version 1.0.1)
// ─────────────────────────────────────────────────────────────
ipmi::RspType<uint8_t, // major
              uint8_t, // minor
              uint8_t  // patch
              >
    cmdGetOnyxVersionHandler()
{
    constexpr uint8_t versionMajor = 1;
    constexpr uint8_t versionMinor = 0;
    constexpr uint8_t versionPatch = 1;

    log<level::INFO>("ONYX OEM: Get Version command received");

    return ipmi::responseSuccess(versionMajor, versionMinor, versionPatch);
}

// ─────────────────────────────────────────────────────────────
// Command 0x02: Add & Subtract
// ─────────────────────────────────────────────────────────────
// Request:  [param1(1B)] [param2(1B)]
// Response: [sum(1B)] [difference(1B)]
//
// Example:
//   ipmitool raw 0x30 0x02 0x0A 0x03
//   Expected response: 0d 07  (10+3=13, 10-3=7)
// ─────────────────────────────────────────────────────────────
ipmi::RspType<uint8_t, // sum
              uint8_t  // difference
              >
    cmdOnyxAddSubHandler(uint8_t param1, uint8_t param2)
{
    log<level::INFO>("ONYX OEM: Add/Sub command received",
                     phosphor::logging::entry("PARAM1=%u", param1),
                     phosphor::logging::entry("PARAM2=%u", param2));

    uint8_t sum  = static_cast<uint8_t>(param1 + param2);
    uint8_t diff = static_cast<uint8_t>(param1 - param2);

    return ipmi::responseSuccess(sum, diff);
}

// ─────────────────────────────────────────────────────────────
// Command 0x03: Echo
// ─────────────────────────────────────────────────────────────
// Request:  [data(1~N Bytes)]
// Response: [length(1B)] [echoed data(N Bytes)]
//
// Returns the input data back prepended with its length byte.
// If no data is provided, returns an error (0xCC - invalid length).
//
// Example:
//   ipmitool raw 0x30 0x03 0x48 0x65 0x6C 0x6C 0x6F
//   Expected response: 05 48 65 6c 6c 6f
//   (length=5, followed by "Hello" in hex)
// ─────────────────────────────────────────────────────────────
ipmi::RspType<uint8_t,              // length of echoed data
              std::vector<uint8_t>  // echoed data
              >
    cmdOnyxEchoHandler(std::vector<uint8_t> data)
{
    if (data.empty())
    {
        log<level::ERR>("ONYX OEM: Echo command received with no data");
        return ipmi::responseReqDataLenInvalid();
    }

    log<level::INFO>("ONYX OEM: Echo command received",
                     phosphor::logging::entry("DATA_LEN=%zu", data.size()));

    uint8_t length = static_cast<uint8_t>(data.size());

    return ipmi::responseSuccess(length, data);
}

// ─────────────────────────────────────────────────────────────
// Registration (called automatically when .so is loaded)
// ─────────────────────────────────────────────────────────────
void registerOnyxOemCommands() __attribute__((constructor));

void registerOnyxOemCommands()
{
    log<level::INFO>("ONYX OEM: Registering OEM IPMI commands");

    // Cmd 0x01 - Get Version (no privilege restriction, any user can query)
    ipmi::registerHandler(ipmi::prioOemBase,
                          netFnOnyxOem,
                          cmdGetOnyxVersion,
                          ipmi::Privilege::User,
                          cmdGetOnyxVersionHandler);

    // Cmd 0x02 - Add/Sub (Operator privilege required)
    ipmi::registerHandler(ipmi::prioOemBase,
                          netFnOnyxOem,
                          cmdOnyxAddSub,
                          ipmi::Privilege::Operator,
                          cmdOnyxAddSubHandler);

    // Cmd 0x03 - Echo (Admin privilege required)
    ipmi::registerHandler(ipmi::prioOemBase,
                          netFnOnyxOem,
                          cmdOnyxEcho,
                          ipmi::Privilege::Admin,
                          cmdOnyxEchoHandler);
}
