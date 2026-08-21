# DC-SCM Hardware Architecture & Device Tree Guide

## 1. Overview
This document specifies the DC-SCM (Data Center Secure Control Module) hardware architecture,
I2C bus topologies, GPIO pin allocations, and sensor Device Tree overlays for AST2600.

## 2. Hardware Pin Mappings
- **I2C Bus 4**: AT24C256 FRU EEPROM (Address 0x50) & TMP421 Thermal Sensor (Address 0x4C)
- **GPIO 184 (GPIOW0)**: System Heartbeat LED (Timer Trigger)
- **GPIO 185 (GPIOW1)**: Fault Indicator LED
- **GPIO 186 (GPIOW2)**: Identification LED
