# x86 Power Sequencing & Hardware Watchdog Guide

## 1. Overview
Hardware guide for Intel/AMD x86 power sequence GPIO states and AST2600 hardware watchdog failover.

## 2. Hardware GPIO Pin Mappings
- **POWER_BUTTON**: Output to Intel/AMD PCH PWRBTN# (GPIOS1)
- **RESET_BUTTON**: Output to Intel/AMD PCH SYS_RESET# (GPIOS2)
- **FM_CPU_SLP_S3_L**: Intel/AMD S3 state input (GPIOP0)
- **FM_CPU_SLP_S5_L**: Intel/AMD S5 state input (GPIOP1)
- **SYS_PWROK**: Main Power Good input (GPIOP2)
- **SPI_FLASH_MUX_SEL**: Dual-SPI CS0 MUX (GPIOQ0)
- **Watchdog Device**: `/dev/watchdog1`
