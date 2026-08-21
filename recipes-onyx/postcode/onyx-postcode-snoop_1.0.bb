SUMMARY = "ONYX eSPI/LPC Snoop POST Code Monitor"
DESCRIPTION = "Captures Port 80h from AST2600 eSPI/LPC snoop character device"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "com.onyx.PostcodeSnoop.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/com.onyx.PostcodeSnoop.service            file://src/onyx-postcode-snoop.cpp"

S = "${WORKDIR}"
