SUMMARY = "ONYX PLDM Type 6 RDE Integration"
DESCRIPTION = "Implement Redfish Device Enablement to mount the NVMe/NIC Redfish tree structure to the BMC."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.PLDM.RDE.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.PLDM.RDE.Onyx.service \
    file://src/onyx-pldm-rde.cpp \
"

S = "${UNPACKDIR}"
