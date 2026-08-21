# Hardware Root of Trust & Secure Boot Guide

## 1. Overview
Hardware guide for AST2600 OTP 8KB eFuse programming, HACE crypto acceleration, and 4-stage boot validation.

## 2. Hardware Specifications
- **OTP Controller**: `/sys/devices/platform/ahb/1e6e5000.otp/` (Register 0x1E6E_5000)
- **HACE Crypto Engine**: `/dev/crypto` (Register 0x1E6D_0000)
- **SCU Secure Boot Register**: `SCU500` / `SCU510`
