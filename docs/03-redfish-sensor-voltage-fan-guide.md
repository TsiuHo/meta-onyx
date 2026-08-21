# Redfish Sensor Data Flow & Fan Control Guide

## 1. Overview
Hardware guide for hwmon sysfs polling, AST2600 SAR ADC resistor calculations, and Redfish telemetry.

## 2. Hardware Pin & Resistor Specifications
- **ADC Channel 0 (P3V3_STBY)**: Divider Ratio = 2.0 (R1=10k, R2=10k, VREF=1.8V)
- **ADC Channel 1 (P5V)**: Divider Ratio = 3.33 (R1=23.3k, R2=10k)
- **ADC Channel 2 (P12V)**: Divider Ratio = 8.0 (R1=70k, R2=10k)
- **PWM Channels**: `pwm1`, `pwm2` (AST2600 PWM Controller 0x1E78_6000)
