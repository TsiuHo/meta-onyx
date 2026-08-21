FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://onyx-virtual-sensors.json"

do_install:append() {
    install -d ${D}${datadir}/phosphor-virtual-sensor
    install -m 0644 ${UNPACKDIR}/onyx-virtual-sensors.json ${D}${datadir}/phosphor-virtual-sensor/virtual_sensor_config.json
}
