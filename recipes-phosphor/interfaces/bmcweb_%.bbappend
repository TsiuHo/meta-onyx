FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
    file://0001-Add-ONYX-OEM-Redfish-endpoint.patch \
    file://0002-Add-ONYX-Fan-Control-OEM-Redfish.patch \
"
