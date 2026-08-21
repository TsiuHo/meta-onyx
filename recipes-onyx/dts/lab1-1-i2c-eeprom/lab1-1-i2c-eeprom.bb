SUMMARY = "Lab 1-1 I2C EEPROM Device Tree documentation/reference"
DESCRIPTION = "This recipe serves as a reference for integrating the I2C EEPROM DTS into the kernel."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# Suitable for BIOS engineers learning Yocto/BitBake:
# In practice, to apply Device Tree modifications to the OpenBMC system,
# It is usually done by creating a linux-aspeed_%.bbappend file,
# And use the patch method, or add a custom .dtsi include,
# Instead of through a separate recipe similar to a plugin like this.
#
# For example, add to meta-onyx/recipes-kernel/linux/linux-aspeed_%.bbappend:
# FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
# SRC_URI += "file://0001-Add-HPM-EEPROM.patch"
#
# This is only for Lab 1-1 source file display and teaching purposes.

FILESEXTRAPATHS:prepend := "${THISDIR}:"

SRC_URI = "file://aspeed-ast2600-evb-lab-eeprom.dts"

S = "${UNPACKDIR}"

do_install() {
    # Only install DTS to /usr/share/labs as reference documents
    install -d ${D}${datadir}/labs/lab1-1
    install -m 0644 ${UNPACKDIR}/aspeed-ast2600-evb-lab-eeprom.dts ${D}${datadir}/labs/lab1-1/
}

FILES:${PN} += "${datadir}/labs/lab1-1/"
