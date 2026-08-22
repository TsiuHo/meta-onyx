SUMMARY = "ONYX MCTP over SMBus Discovery Service"
DESCRIPTION = "Discovers GPUs and NVSwitches on HGX Baseboard and configures MCTP routing."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.MCTP.SMBus.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.MCTP.SMBus.Onyx.service \
    file://src/onyx-smbus-mctp.cpp \
"

S = "${UNPACKDIR}"
