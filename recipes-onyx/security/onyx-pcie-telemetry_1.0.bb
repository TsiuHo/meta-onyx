SUMMARY = "ONYX MCTP over PCIe VDM High-Speed Telemetry"
DESCRIPTION = "Collect high-speed telemetry data of GPU/DPU via PCIe VDM, breaking through traditional I2C bandwidth bottlenecks."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.Telemetry.PCIe.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.Telemetry.PCIe.Onyx.service \
    file://src/onyx-pcie-telemetry.cpp \
"

S = "${UNPACKDIR}"
