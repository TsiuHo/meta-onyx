SUMMARY = "ONYX PLDM Type 5 Firmware Update Device Daemon"
DESCRIPTION = "Production hardware interface for SPI flash programming and DSP0267"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.Software.OnyxBios.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/xyz.openbmc_project.Software.OnyxBios.service            file://src/onyx-pldm-fwup.cpp"

S = "${UNPACKDIR}"
