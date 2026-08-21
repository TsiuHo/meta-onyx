SUMMARY = "ONYX Voltage Telemetry Service"
DESCRIPTION = "Reads AST2600 SAR ADC channels from Linux IIO subsystem"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.OnyxVoltageMonitor.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/xyz.openbmc_project.OnyxVoltageMonitor.service            file://src/onyx-voltage-monitor.cpp"

S = "${WORKDIR}"
