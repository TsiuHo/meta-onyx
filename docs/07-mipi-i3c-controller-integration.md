# MIPI I3C Master Controller Integration Guide

## 1. Overview
Hardware guide for AST2600 MIPI I3C v1.1 master controller, DAA protocol, and In-Band Interrupts.

## 2. Hardware Specifications
- **I3C Character Device**: `/dev/i3c-0` (Register 0x1E7A_0000)
- **Bus Speed**: 12.5 MHz (SDR) / 25 MHz (HDR)
- **Sysfs Bus Directory**: `/sys/bus/i3c/devices`
