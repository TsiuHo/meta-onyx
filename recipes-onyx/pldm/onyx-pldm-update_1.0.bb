SUMMARY = "ONYX PLDM Type 5 Seamless FW Update"
DESCRIPTION = "Implement compliant out-of-band firmware transmission, verification, and update for GPU/NVSwitch."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.PLDM.FWUpdate.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.PLDM.FWUpdate.Onyx.service \
    file://src/onyx-pldm-update.cpp \
"

S = "${UNPACKDIR}"
