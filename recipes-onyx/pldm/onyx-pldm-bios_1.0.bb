SUMMARY = "ONYX PLDM BIOS Configuration Manager"
DESCRIPTION = "Production hardware interface for DMTF DSP0247 BIOS configuration"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig

DEPENDS = "sdbusplus phosphor-logging boost"
RDEPENDS:${PN} = "pldm"

SRC_URI = "file://meson.build            file://LICENSE            file://src/onyx-pldm-bios.cpp"

S = "${UNPACKDIR}"
