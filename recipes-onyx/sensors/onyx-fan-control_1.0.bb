SUMMARY = "ONYX Hardware Fan Controller"
DESCRIPTION = "Drives AST2600 PWM duty cycles with tachometer closed loop"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "com.onyx.FanControl.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/com.onyx.FanControl.service            file://src/onyx-fan-control.cpp"

S = "${UNPACKDIR}"
