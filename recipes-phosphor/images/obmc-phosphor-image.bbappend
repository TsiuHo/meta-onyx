# Add ONYX OEM packages and production hardware daemons to the image
OBMC_IMAGE_EXTRA_INSTALL:append = " \
    onyx-ipmi \
    onyx-service \
    phosphor-gpio-monitor-monitor \
    onyx-pldm-config \
    onyx-pldm-bios \
    onyx-pldm-fwup \
    onyx-sensor-monitor \
    onyx-voltage-monitor \
    onyx-fan-control \
    onyx-postcode-snoop \
    onyx-rot-manager \
    onyx-vmedia-manager \
    onyx-i3c-manager \
    onyx-ncsi-manager \
    onyx-power-wdt \
    onyx-pcie-mctp \
    pldm \
    pldmtool \
    phosphor-virtual-sensor \
    entity-manager \
    i2c-tools \
    libgpiod-tools \
"
