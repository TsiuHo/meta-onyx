SUMMARY = "ONYX OEM IPMI commands for AST2600"
DESCRIPTION = "Custom OEM IPMI command provider for ONYX AST2600 platform. \
Provides three commands under OEM NetFn 0x30: \
GetVersion (0x01), AddSub (0x02), and Echo (0x03)."

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=172b200b4a750f3ca17e24e78120edd7"

# Use local source files shipped alongside this recipe
SRC_URI = " \
    file://src/onyx-ipmi.cpp \
    file://meson.build \
    file://LICENSE \
"

S = "${UNPACKDIR}"

PV = "1.0.1"

# Build dependencies
DEPENDS += "phosphor-ipmi-host"
DEPENDS += "phosphor-logging"

# Use Meson build system (standard for OpenBMC projects)
inherit meson pkgconfig

# Create symlinks in host-ipmid and net-ipmid provider directories
# so this provider is loaded for both in-band (host) and out-of-band (net)
inherit obmc-phosphor-ipmiprovider-symlink

LIBRARY_NAMES = "libonyxoemcmds.so"

HOSTIPMI_PROVIDER_LIBRARY += "${LIBRARY_NAMES}"
NETIPMI_PROVIDER_LIBRARY += "${LIBRARY_NAMES}"

# Package the shared library for runtime
FILES:${PN}:append = " ${libdir}/ipmid-providers/lib*${SOLIBS}"
FILES:${PN}:append = " ${libdir}/host-ipmid/lib*${SOLIBS}"
FILES:${PN}:append = " ${libdir}/net-ipmid/lib*${SOLIBS}"
FILES:${PN}-dev:append = " ${libdir}/ipmid-providers/lib*${SOLIBSDEV}"
