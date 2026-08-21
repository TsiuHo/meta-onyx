# Add ONYX OEM packages and Lab exercises to the image
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-ipmi onyx-service phosphor-gpio-monitor-monitor"

# ─── Lab Exercise Packages ───
# Lab 2: PLDM/MCTP exercises
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-pldm-config"
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-pldm-bios-sim"
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-pldm-fwup-sim"

# Lab 3: Redfish/Sensor exercises
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-sensor-init"
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-voltage-monitor"
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-fan-control"

# Ensure PLDM tools are included
OBMC_IMAGE_EXTRA_INSTALL:append = " pldm pldmtool"

# Ensure virtual sensor engine is included
OBMC_IMAGE_EXTRA_INSTALL:append = " phosphor-virtual-sensor"

# Ensure Entity Manager is included for sensor discovery
OBMC_IMAGE_EXTRA_INSTALL:append = " entity-manager"

# I2C and GPIO tools for Lab 1 exercises, plus curl for testing
OBMC_IMAGE_EXTRA_INSTALL:append = " i2c-tools libgpiod-tools curl"

# ─── Advanced Lab Exercise Packages (Lab 4~10) ───
# Lab 4: eSPI/LPC Snoop POST Code monitor
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-postcode-snoop"

# Lab 5: Hardware Root of Trust & Secure Boot
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-rot-tool"

# Lab 6: Video Engine KVM & Virtual Media
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-vmedia-sim"

# Lab 7: MIPI I3C DAA & IBI
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-i3c-sim"

# Lab 8: NC-SI Sideband Network
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-ncsi-sim"

# Lab 9: x86 Power Sequencing & WDT Failover
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-power-wdt"

# Lab 10: PCIe VDM High-Speed MCTP
OBMC_IMAGE_EXTRA_INSTALL:append = " onyx-pcie-mctp"
