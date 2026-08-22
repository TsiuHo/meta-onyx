SUMMARY = "ONYX PLDM Type 2 GPU/NVSwitch Thermal & Power"
DESCRIPTION = "Read GPU temperature Sensor and implement Power Capping power control."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.PLDM.Thermal.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.PLDM.Thermal.Onyx.service \
    file://src/onyx-pldm-thermal.cpp \
"

S = "${UNPACKDIR}"
