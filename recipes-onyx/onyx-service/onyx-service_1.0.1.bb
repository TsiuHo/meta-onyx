SUMMARY = "ONYX OEM D-Bus Information Service"
DESCRIPTION = "A D-Bus daemon that publishes custom ONYX OEM properties for bmcweb Redfish to consume."

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=172b200b4a750f3ca17e24e78120edd7"

# Use local source files shipped alongside this recipe
SRC_URI = " \
    file://src/main.cpp \
    file://meson.build \
    file://LICENSE \
    file://service/com.onyx.OemInfo.service \
"

S = "${UNPACKDIR}"

PV = "1.0.1"

# Build dependencies
DEPENDS += "sdbusplus"
DEPENDS += "boost"
DEPENDS += "systemd"

# Use Meson build system (standard for OpenBMC projects)
inherit meson pkgconfig systemd

SYSTEMD_SERVICE:${PN} = "com.onyx.OemInfo.service"

FILES:${PN} += "${systemd_system_unitdir}/com.onyx.OemInfo.service"
