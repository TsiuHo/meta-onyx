FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://onyx-lab-baseboard.json"

do_install:append() {
    install -d ${D}${datadir}/entity-manager/configurations
    install -m 0644 ${UNPACKDIR}/onyx-lab-baseboard.json ${D}${datadir}/entity-manager/configurations/
}
