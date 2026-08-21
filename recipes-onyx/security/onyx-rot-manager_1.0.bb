SUMMARY = "ONYX Hardware Root of Trust Manager"
DESCRIPTION = "Reads AST2600 OTP eFuse registers and HACE Crypto Engine state"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.Control.Security.RoT.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/xyz.openbmc_project.Control.Security.RoT.service            file://src/onyx-rot-manager.cpp"

S = "${UNPACKDIR}"
