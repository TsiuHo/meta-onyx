SUMMARY = "ONYX PCIe VDM High-Speed MCTP Manager"
DESCRIPTION = "Transmits 64-byte PCIe TLP VDM packets via AST2600 PCIe EP driver"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=39d9df46ef77c8fdb80ccdeb89949633"

inherit meson pkgconfig systemd

DEPENDS = "sdbusplus phosphor-logging boost"
SYSTEMD_SERVICE:${PN} = "xyz.openbmc_project.MCTP.PCIeVDM.Onyx.service"

SRC_URI = "file://meson.build            file://LICENSE            file://service/xyz.openbmc_project.MCTP.PCIeVDM.Onyx.service            file://src/onyx-pcie-mctp.cpp"

S = "${UNPACKDIR}"
