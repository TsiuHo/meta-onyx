SUMMARY = "ONYX NC-SI Network Management Service"
DESCRIPTION = "Manages Linux Netlink NC-SI subsystem for multi-channel failover"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.Network.NCSI.Onyx.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/xyz.openbmc_project.Network.NCSI.Onyx.service            file://src/onyx-ncsi-manager.cpp"

S = "${WORKDIR}"
