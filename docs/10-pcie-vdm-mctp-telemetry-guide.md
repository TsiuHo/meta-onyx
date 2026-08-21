# PCIe VDM High-Speed MCTP Telemetry Guide

## 1. Overview
Hardware guide for AST2600 PCIe Gen2 Endpoint controller and DMTF DSP0238 MCTP over PCIe VDM.

## 2. Hardware Specifications
- **Device Node**: `/dev/aspeed-pcie-vdm` (Register 0x1E6E_0000)
- **Vendor ID**: `0x1AB4` (DMTF Pre-OS)
- **TLP Header Format**: 4DW Header with Data (Type 1 Route by ID: 01111110b)
