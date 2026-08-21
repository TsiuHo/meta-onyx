# PLDM BIOS Configuration & Firmware Update Guide

## 1. Overview
Production guide for DMTF DSP0247 (PLDM BIOS Control) and DSP0267 (PLDM Firmware Update).

## 2. Hardware Interfaces
- **KCS Channel 3**: `/dev/aspeed-kcs3` (Host BIOS to BMC MCTP transport)
- **Host BIOS SPI Flash**: `/dev/mtd/bios` (SPI1 controller CS0)
- **SPI MUX GPIO**: `SPI_FLASH_MUX_SEL` (GPIOS0)
- **HACE Crypto Acceleration**: `/dev/crypto` (SHA-256/384, RSA-4096)
