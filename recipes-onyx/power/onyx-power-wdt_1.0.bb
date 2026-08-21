SUMMARY = "ONYX Power Sequencing & Hardware Watchdog Manager"
DESCRIPTION = "Manages motherboard power GPIOs and AST2600 Hardware Watchdog"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.State.Chassis.Onyx.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/xyz.openbmc_project.State.Chassis.Onyx.service            file://src/onyx-power-wdt.cpp"

S = "${UNPACKDIR}"
