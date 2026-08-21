SUMMARY = "ONYX PLDM BIOS Attributes Configuration"
DESCRIPTION = "Provides BIOS attributes JSON configuration for PLDM"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

SRC_URI = "file://bios_attrs.json"

S = "${UNPACKDIR}"

do_install() {
    install -d ${D}${datadir}/pldm/bios
    install -m 0644 ${UNPACKDIR}/bios_attrs.json ${D}${datadir}/pldm/bios/
}

FILES:${PN} += "${datadir}/pldm/bios/bios_attrs.json"

RDEPENDS:${PN} += "pldm"
