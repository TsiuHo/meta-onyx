SUMMARY = "ONYX MIPI I3C Master Controller Service"
DESCRIPTION = "Interfaces with Linux kernel I3C subsystem (/dev/i3c-0)"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.I3C.Onyx.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/xyz.openbmc_project.I3C.Onyx.service            file://src/onyx-i3c-manager.cpp"

S = "${UNPACKDIR}"
