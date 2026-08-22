SUMMARY = "ONYX SPDM Authentication for GPU/DPU"
DESCRIPTION = "Verify NVIDIA chip identity via SPDM certificates and Challenge-Response."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.SPDM.Auth.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.SPDM.Auth.Onyx.service \
    file://src/onyx-spdm-auth.cpp \
"

S = "${UNPACKDIR}"
