FILESEXTRAPATHS:prepend := "${THISDIR}/phosphor-gpio-monitor:"

SRC_URI:append = " file://onyx-phosphor-multi-gpio-monitor.json"

do_install:append() {
    install -d ${D}${datadir}/phosphor-gpio-monitor
    install -m 0644 ${UNPACKDIR}/onyx-phosphor-multi-gpio-monitor.json \
                    ${D}${datadir}/phosphor-gpio-monitor/phosphor-multi-gpio-monitor.json
}
