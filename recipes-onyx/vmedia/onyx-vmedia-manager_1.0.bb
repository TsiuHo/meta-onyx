SUMMARY = "ONYX Video & USB Virtual Media Manager"
DESCRIPTION = "Controls AST2600 Video Engine (/dev/video0) and USB Gadget ConfigFS"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.VirtualMedia.Onyx.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/xyz.openbmc_project.VirtualMedia.Onyx.service            file://src/onyx-vmedia-manager.cpp"

S = "${UNPACKDIR}"
