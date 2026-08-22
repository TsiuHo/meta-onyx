SUMMARY = "ONYX SPDM Attestation & Measurement"
DESCRIPTION = "Read the firmware hash values of GPU and NVSwitch at boot to ensure they haven't been tampered with."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.SPDM.Attestation.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.SPDM.Attestation.Onyx.service \
    file://src/onyx-spdm-attestation.cpp \
"

S = "${UNPACKDIR}"
