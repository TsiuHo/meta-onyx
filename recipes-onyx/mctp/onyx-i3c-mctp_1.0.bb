SUMMARY = "ONYX MCTP over I3C Dynamic Address Assignment"
DESCRIPTION = "Implement I3C Dynamic Address Assignment (DAA) and In-Band Interrupt (IBI) for low-latency MCTP transmission."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.MCTP.I3C.Onyx.service"

SRC_URI = " \
    file://meson.build \
    file://LICENSE \
    file://service/xyz.openbmc_project.MCTP.I3C.Onyx.service \
    file://src/onyx-i3c-mctp.cpp \
"

S = "${UNPACKDIR}"
