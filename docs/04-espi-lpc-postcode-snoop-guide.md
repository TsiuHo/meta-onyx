# eSPI/LPC Snoop POST Code & Watchdog Guide

## 1. Overview
Hardware guide for capturing x86 Host BIOS Port 80h/81h boot progression via AST2600 eSPI Snoop controller.

## 2. Hardware Specifications
- **Device Node**: `/dev/aspeed-lpc-snoop0` (AST2600 Register base 0x1E78_9080)
- **Host Reset Pin**: `HOST_PLTRST_N` (GPIOP4)
