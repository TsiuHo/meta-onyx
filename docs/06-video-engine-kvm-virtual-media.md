# Video Engine KVM & Virtual Media Architecture Guide

## 1. Overview
Hardware guide for AST2600 Video Engine (V4L2) and USB 2.0 Virtual Hub (aspeed-vhub) mass storage.

## 2. Hardware Specifications
- **Video Engine V4L2 Device**: `/dev/video0` (Register 0x1E70_0000)
- **USB 2.0 Virtual Hub Driver**: `aspeed-vhub` (Register 0x1E6A_0000)
- **ConfigFS LUN Node**: `/sys/kernel/config/usb_gadget/g1/functions/mass_storage.usb0/lun.0/file`
