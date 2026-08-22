SUMMARY = "ONYX Secure MCTP (SPDM Secure Session)"
DESCRIPTION = "Establish an encrypted MCTP transmission channel to prevent I2C/PCIe bus eavesdropping and malicious command injection."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.SPDM.SecureMCTP.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.SPDM.SecureMCTP.Onyx.service \
    file://src/onyx-secure-mctp.cpp \
"

S = "${UNPACKDIR}"
