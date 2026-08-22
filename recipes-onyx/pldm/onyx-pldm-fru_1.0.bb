SUMMARY = "ONYX PLDM Type 4 HGX Baseboard FRU Data"
DESCRIPTION = "Read FRU information of GPU and NVSwitch, and write to Inventory DBus."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.PLDM.FRU.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.PLDM.FRU.Onyx.service \
    file://src/onyx-pldm-fru.cpp \
"

S = "${UNPACKDIR}"
