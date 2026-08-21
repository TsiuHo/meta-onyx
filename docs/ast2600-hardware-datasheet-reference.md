# ASPEED AST2600 伺服器管理處理器 (BMC) 硬體規格與架構參考手冊

> **文件版本**：v1.0 | **整理日期**：2026-08-21
> **適用晶片**：ASPEED AST2600 A1 / A2 / A3 7th Generation Server Management Processor
> **儲存路徑**：`meta-onyx/docs/ast2600-hardware-datasheet-reference.md`

---

## 目錄

1. [SoC 總體架構與特性](#1-soc-總體架構與特性)
2. [晶片方塊圖 (Block Diagram)](#2-晶片方塊圖-block-diagram)
3. [處理器核心與記憶體次系統](#3-處理器核心與記憶體次系統)
4. [晶片記憶體映射表 (System Memory Map)](#4-晶片記憶體映射表-system-memory-map)
5. [主要週邊控制器 (Peripheral Controllers)](#5-主要週邊控制器-peripheral-controllers)
   - 5.1 [系統控制單元 (SCU & Reset)](#51-系統控制單元-scu--reset)
   - 5.2 [Flash 記憶體控制器 (FMC / SPI1 / SPI2)](#52-flash-記憶體控制器-fmc--spi1--spi2)
   - 5.3 [I2C / SMBus / PMBus 控制器 (16 組)](#53-i2c--smbus--pmbus-控制器-16-組)
   - 5.4 [I3C 控制器 (MIPI I3C v1.1, 4 組)](#54-i3c-控制器-mipi-i3c-v11-4-組)
   - 5.5 [Host 通訊介面 (eSPI / LPC / KCS / BT / MBOX)](#55-host-通訊介面-espi--lpc--kcs--bt--mbox)
   - 5.6 [PCIe 與 MCTP over PCIe VDM](#56-pcie-與-mctp-over-pcie-vdm)
   - 5.7 [網路控制器與 NC-SI (4x Gigabit MAC)](#57-網路控制器與-nc-si-4x-gigabit-mac)
   - 5.8 [類比/數位轉換器 (16-ch 10-bit ADC)](#58-類比數位轉換器-16-ch-10-bit-adc)
   - 5.9 [PWM 輸出與風扇轉速計 (16-ch PWM / 16-ch Tach)](#59-pwm-輸出與風扇轉速計-16-ch-pwm--16-ch-tach)
   - 5.10 [通用輸入輸出 (GPIO & SGPIO, 228 pins)](#510-通用輸入輸出-gpio--sgpio-228-pins)
   - 5.11 [2D 繪圖與視訊壓縮引擎 (Remote KVM Video Engine)](#511-2d-繪圖與視訊壓縮引擎-remote-kvm-video-engine)
   - 5.12 [硬體安全與 Root of Trust (OTP / Crypto / Secure Boot)](#512-硬體安全與-root-of-trust-otp--crypto--secure-boot)
   - 5.13 [看門狗計時器 (Watchdog Timer, 4 組)](#513-看門狗計時器-watchdog-timer-4-組)
   - 5.14 [通用非同步收發器 (UART 1~5 + VUART + HUART)](#514-通用非同步收發器-uart-15--vuart--huart)
   - 5.15 [USB 2.0 控制器 (Host + Virtual Device)](#515-usb-20-控制器-host--virtual-device)
6. [開發與實驗對照總表](#6-開發與實驗對照總表)

---

## 1. SoC 總體架構與特性

ASPEED **AST2600** 是第 7 代伺服器管理處理器（Baseboard Management Controller, BMC），採用 **28nm 製程**，專為現代雲端資料中心、企業級伺服器、邊緣運算節點及 DC-SCM 模組設計。

| 規格類別 | 技術參數 | 說明 |
| :--- | :--- | :--- |
| **製程** | TSMC 28nm HPC+ | 相比 AST2500 (40nm) 功耗更低、運算效能提升 2.5 倍 |
| **主要處理器** | 雙核心 ARM Cortex-A7 @ 1.2 GHz | 支援 ARMv7-A 指令集、NEON SIMD、VFPv4 浮點運算、SMP 對稱多核心 |
| **快取記憶體** | L1: 32KB I-Cache + 32KB D-Cache / 核心<br>L2: 512KB 共用快取 | 提升 OpenBMC 多守護行程 (daemons) 平行處理效能 |
| **輔助協處理器** | 2 顆 Cortex-M3 (800MHz / 200MHz) | 用於即時控制、I3C 協定處理、低功耗電源管理與安全監控 |
| **系統記憶體** | DDR4-2400 / DDR4-1600 / DDR3-1600 (32-bit / 16-bit) | 最大支援 2GB 容量，支援 ECC 記憶體校正 |
| **安全根信任** | Hardware Root of Trust (RoT) | 內建 OTP (One-Time Programmable) 安全金鑰儲存、RSA-2048/3072/4096、ECC、AES-256、SHA-2/SHA-3 加速引擎 |
| **封裝形式** | 19mm × 19mm TFBGA (408-ball / 532-ball) | 0.8mm 球距，符合工業級溫度 (-40°C ~ 85°C) |

---

## 2. 晶片方塊圖 (Block Diagram)

```text
┌────────────────────────────────────────────────────────────────────────────────────────┐
│                                 ASPEED AST2600 SoC                                     │
├────────────────────────────────────────────────────────────────────────────────────────┤
│  ┌───────────────────────────┐  ┌───────────────────────────┐  ┌────────────────────┐  │
│  │ Dual ARM Cortex-A7 @1.2G  │  │ Dual ARM Cortex-M3 Coproc │  │ Crypto & RoT Engine│  │
│  │ (32KB L1 I/D + 512KB L2)  │  │ (Real-time Tasks & I3C)   │  │ (RSA/ECC/AES/SHA)  │  │
│  └─────────────┬─────────────┘  └─────────────┬─────────────┘  └─────────┬──────────┘  │
│                │ AXI Bus (64-bit)             │                          │             │
│  ══════════════╪══════════════════════════════╪══════════════════════════╪═══════════  │
│                │                              │                          │             │
│  ┌─────────────▼─────────────┐  ┌─────────────▼─────────────┐  ┌─────────▼──────────┐  │
│  │   DDR4/DDR3 Controller    │  │  Flash Memory Controller  │  │  Secure OTP Memory │  │
│  │  (Up to 2GB, 32-bit+ECC)  │  │  (FMC + SPI1 + SPI2)      │  │  (Keys & Config)   │  │
│  └───────────────────────────┘  └───────────────────────────┘  └────────────────────┘  │
│                                                                                        │
│  ─────────────────────────────── 週邊通訊介面 ─────────────────────────────────────────  │
│                                                                                        │
│  ┌─────────────────────────┐  ┌─────────────────────────┐  ┌────────────────────────┐  │
│  │ 16x I2C / SMBus / PMBus │  │ 4x MIPI I3C Controllers │  │ eSPI / LPC Controller  │  │
│  │ (Master/Slave, DMA, 1M) │  │ (SDR/HDR, IBI, 12.5MHz) │  │ (KCS, BT, MBOX, Snoop) │  │
│  └─────────────────────────┘  └─────────────────────────┘  └────────────────────────┘  │
│  ┌─────────────────────────┐  ┌─────────────────────────┐  ┌────────────────────────┐  │
│  │ 4x Gigabit Ethernet MAC │  │ PCIe Gen2 Root / Endpoint│  │ USB 2.0 Host / Device  │  │
│  │ (RGMII/RMII, NC-SI)     │  │ (MCTP over PCIe VDM)    │  │ (Virtual KVM & vMedia) │  │
│  └─────────────────────────┘  └─────────────────────────┘  └────────────────────────┘  │
│  ┌─────────────────────────┐  ┌─────────────────────────┐  ┌────────────────────────┐  │
│  │ 16-channel 10-bit ADC   │  │ 16x PWM & 16x Tachometer│  │ 228x GPIO & SGPIO      │  │
│  │ (Voltage & Temp Sense)  │  │ (Multi-zone Fan Control)│  │ (Pass-through & Irq)   │  │
│  └─────────────────────────┘  └─────────────────────────┘  └────────────────────────┘  │
│  ┌─────────────────────────┐  ┌─────────────────────────┐  ┌────────────────────────┐  │
│  │ 2D Video & Compression  │  │ 5x UART + VUART + HUART │  │ 4x Watchdog Timers     │  │
│  │ (Remote KVM 1920x1200)  │  │ (SOL, Debug Console)    │  │ (System Reset & Recovery│  │
│  └─────────────────────────┘  └─────────────────────────┘  └────────────────────────┘  │
└────────────────────────────────────────────────────────────────────────────────────────┘
```

---

## 3. 處理器核心與記憶體次系統

### 3.1 雙核心 ARM Cortex-A7
* **運作時脈**：最高 1.2 GHz（可由 SCU 暫存器調頻）。
* **架構特性**：ARMv7-A 體系結構，支援 40-bit 大實體位址擴充（LPAE）。
* **中斷控制器**：ARM GIC-400 (Generic Interrupt Controller v2)，支援 128 組以上的外部週邊中斷線。
* **計時器**：ARM Generic Timer 架構與 8 組 SoC 專用 32-bit 計時器。

### 3.2 輔助處理器架構
* **Cortex-M3 #1 (800MHz)**：專職處理高速 I3C 匯流排狀態機與資料緩衝。
* **Cortex-M3 #2 (200MHz)**：低功耗常時運作（Always-on domain），負責待機電源監控、Wake-on-LAN 與安全狀態探測。

---

## 4. 晶片記憶體映射表 (System Memory Map)

AST2600 採用統一實體記憶體映射位址空間：

| 記憶體區段位址 | 大小 | 裝置 / 控制器名稱 | 說明 |
| :--- | :--- | :--- | :--- |
| `0x0000_0000` ~ `0x0000_FFFF` | 64 KB | Boot ROM / Internal SRAM | 晶片開機啟動韌體 (BootROM) 與初始堆疊 |
| `0x1000_0000` ~ `0x17FF_FFFF` | 128 MB | Internal SRAM / Crypto Buffer | 加密運算快取區與安全暫存區 |
| `0x2000_0000` ~ `0x2FFF_FFFF` | 256 MB | FMC (SPI Boot Flash Direct Map) | BMC 主要開機 SPI Flash 記憶體對應區 |
| `0x3000_0000` ~ `0x37FF_FFFF` | 128 MB | SPI1 (Host BIOS Flash Direct Map)| Host BIOS SPI Flash 記憶體對應區 |
| `0x3800_0000` ~ `0x3FFF_FFFF` | 128 MB | SPI2 (Secondary Flash Direct Map)| 次要週邊 SPI Flash 對應區 |
| `0x8000_0000` ~ `0xFFFF_FFFF` | 2 GB | DRAM (DDR4 / DDR3 SDRAM) | 系統主要實體記憶體空間 |
| **`0x1E60_0000` ~ `0x1E7F_FFFF`** | **2 MB** | **週邊暫存器基底區 (Peripherals Base)** | **所有晶片控制器暫存器區 (詳見下表)** |

### 週邊控制器暫存器基底位址 (AHB / APB Register Base)

| 基底位址 (Base Address) | 控制器名稱 | 控制器功能描述 |
| :--- | :--- | :--- |
| `0x1E62_0000` | **FMC Controller** | Flash Memory Controller 暫存器區 |
| `0x1E63_0000` | **SPI1 Controller** | Host SPI Controller 暫存器區 |
| `0x1E63_1000` | **SPI2 Controller** | Peripheral SPI Controller 暫存器區 |
| `0x1E65_0000` | **SDRAM Controller** | DDR4/DDR3 記憶體時序與 PHY 控制暫存器 |
| `0x1E6C_0000` | **Crypto Engine (HACE)** | Hash & Crypto Engine (SHA/AES/RSA 加速器) |
| `0x1E6E_0000` | **eMMC / SD Controller** | eMMC 5.1 / SD 3.0 主控制器 |
| `0x1E6E_2000` | **SCU (System Control Unit)** | 系統時脈、重置、腳位複用 (Pinmux)、Strapping |
| `0x1E72_0000` | **Watchdog Controller** | WDT1 ~ WDT4 控制暫存器 |
| `0x1E78_0000` | **GPIO Controller (Bank A~AC)**| 228 支 GPIO 方向、資料、中斷與去彈跳控制 |
| `0x1E78_4000` | **Real Time Clock (RTC)** | 晶片硬體 RTC 計時器 |
| `0x1E78_5000` | **Timer Controller** | 8 組通用 32-bit Timer 計時器 |
| `0x1E78_7000` | **PWM & Fan Tachometer** | 16 組 PWM 輸出與 16 組 Tach 轉速捕捉 |
| `0x1E78_8000` | **LPC / eSPI Controller** | LPC/eSPI 匯流排控制、KCS、BT、MBOX、Port 80h |
| `0x1E78_A000` | **I2C Controller Global** | 16 組 I2C 匯流排全域控制暫存器 |
| `0x1E78_A080` ~ `0x1E78_AF80` | **I2C Bus 0 ~ 15** | 個別 I2C 通道控制暫存器 (每組偏移 0x80) |
| `0x1E7A_0000` | **I3C Controller Global** | 4 組 MIPI I3C 控制器暫存器 |
| `0x1E66_0000` / `0x1E68_0000` | **Gigabit Ethernet MAC 1~4** | 4 組 10/100/1000 Mbps 乙太網路 MAC 控制器 |
| `0x1E6E_9000` | **ADC Controller** | 16 通道 10-bit 類比數位轉換器 |
| `0x1E70_0000` | **Video Engine (VE)** | Remote KVM 視訊擷取與硬體壓縮引擎 |
| `0x1E78_3000` ~ `0x1E78_3400` | **UART 1 ~ 5** | 序列通訊埠 (Debug Console, Host SOL) |

---

## 5. 主要週邊控制器 (Peripheral Controllers)

### 5.1 系統控制單元 (SCU & Reset)
* **基底位址**：`0x1E6E_2000`
* **核心功能**：
  * **時脈管理**：HPLL (CPU 時脈)、MPLL (DRAM 時脈)、DPLL (Display)、EPLL (PCIe/USB)、APLL (Audio) 與各週邊除頻器配置。
  * **硬體重置控制**：個別 IP 區塊重置（軟體可強制重置 I2C、MAC、eSPI 等單一模組）。
  * **Pinmux 腳位複用設定**：透過 `SCU400` ~ `SCU4F0` 暫存器配置多功能腳位（例如 GPIO 與 I2C/UART/PWM 之間的切換）。
  * **硬體 Strapping 狀態讀取**：`SCU500` / `SCU510` 暫存器記錄開機時由電阻拉高的硬體設定（開機媒介、除錯 UART 選擇等）。

### 5.2 Flash 記憶體控制器 (FMC / SPI1 / SPI2)
* **基底位址**：FMC (`0x1E62_0000`), SPI1 (`0x1E63_0000`), SPI2 (`0x1E63_1000`)
* **硬體架構**：
  * **FMC (Flash Memory Controller)**：支援最多 3 個 Chip Select (`CS0` ~ `CS2`)，最高 200MHz SPI-NOR 時脈，支援 Dual / Quad / Octal SPI 模式，負責 BMC 自身 Linux/U-Boot 啟動。
  * **SPI1 (Host SPI Controller)**：支援 2 個 Chip Select，直連 Host 主機板的 BIOS SPI Flash，支援硬體 SPI MUX 切換（BIOS 存取 vs. BMC 存取）。
  * **SPI2 (Peripheral SPI)**：用於連接外接 CPLD、TPM 2.0 或安全晶片。

### 5.3 I2C / SMBus / PMBus 控制器 (16 組)
* **基底位址**：`0x1E78_A000`
* **控制器特性**：
  * 提供 16 個獨立硬體 I2C 控制通道（`i2c0` ~ `i2c15`）。
  * 支援 **Standard (100 kbps)**、**Fast (400 kbps)**、**Fast-Plus (1 Mbps)** 與 **High-Speed (3.4 Mbps)**。
  * 內建 **Scatter-Gather DMA Engine**，支援大封包傳輸免 CPU 介入。
  * 支援 **Multi-Master 仲裁機制**、**SMBus 3.0 硬體 PEC (Packet Error Code)** 校驗與 **PMBus** 協定。
  * 支援 **I2C Slave 模式** 與 **Slave Buffer Mode**，可讓 BMC 扮演 I2C 從屬設備響應 Host 端查詢。

### 5.4 I3C 控制器 (MIPI I3C v1.1, 4 組)
* **基底位址**：`0x1E7A_0000`
* **特性**：
  * 4 組獨立硬體 MIPI I3C Master / Slave 介面。
  * 支援高達 **12.5 MHz** SDR 模式與 HDR-DDR 模式（傳輸速率高達 33.3 Mbps）。
  * 支援 **動態位址分配 (Dynamic Address Assignment, DAA)**。
  * 支援 **帶內中斷 (In-Band Interrupt, IBI)**，無需額外 GPIO 中斷訊號線即可接收感測器異常告警。
  * 由內建專屬 Cortex-M3 協處理器直接驅動，卸載 A7 主核心負載。

### 5.5 Host 通訊介面 (eSPI / LPC / KCS / BT / MBOX)
* **基底位址**：`0x1E78_8000`
* **介面支援**：
  * **eSPI (Enhanced Serial Peripheral Interface)**：支援 1x/2x/4x 模式，最高 66MHz 時脈，支援 Peripheral Channel、Virtual Wire Channel、OOB (Out-of-Band) Channel 及 Flash Access Channel。
  * **LPC (Low Pin Count)**：傳統 PC 相容 33MHz LPC 匯流排。
  * **KCS (Keyboard Controller Style)**：4 組獨立硬體 KCS 介面（KCS1~KCS4），用於傳統 IPMI 協定通訊。
  * **BT (Block Transfer)**：高頻寬區塊傳輸介面，適合大量 IPMI 資料傳輸。
  * **LPC/eSPI Snoop**：硬體 Port 80h / 81h POST Code 捕捉器，內建 256-byte FIFO 緩衝區，可在系統死當前完整記錄開機除錯碼。
  * **LPC Mailbox (MBOX)**：提供 16 個 32-bit 資料暫存器與雙向中斷機制，供 BIOS 與 BMC 進行自訂協議通訊。

### 5.6 PCIe 與 MCTP over PCIe VDM
* **特性**：
  * PCIe Gen2 (5.0 Gbps) 支援，可配置為 **Root Complex (RC)** 或 **End Point (EP)**。
  * 內建 **MCTP over PCIe VDM (Vendor Defined Messages)** 硬體過濾與收發緩衝區。
  * 支援 BMC-to-Host 高速 PLDM/MCTP 通訊（傳輸速度可達 500 MB/s 以上，遠超過 I2C/KCS）。

### 5.7 網路控制器與 NC-SI (4x Gigabit MAC)
* **基底位址**：`0x1E66_0000` (MAC1/2), `0x1E68_0000` (MAC3/4)
* **硬體能力**：
  * 4 組 10/100/1000 Mbps Ethernet MAC 控制器。
  * 支援 RGMII 與 RMII 實體層 (PHY) 介面。
  * **NC-SI (Network Controller Sideband Interface)**：支援硬體 NC-SI 1.1，可透過共享的主機板網卡（Intel / Broadcom / Mellanox NIC）共用網路孔進行 Out-of-Band 遠端管理。
  * 支援 IEEE 1588 硬體時間戳記與硬體 IPv4/IPv6 Checksum 卸載。

### 5.8 類比/數位轉換器 (16-ch 10-bit ADC)
* **基底位址**：`0x1E6E_9000`
* **規格與架構**：
  * 16 組獨立類比輸入通道（ADC0 ~ ADC15）。
  * 10-bit 解析度，取樣率最高可達 **1 MSPS**。
  * 參考電壓範圍：0V ~ 1.8V / 2.5V。
  * 內建硬體平均濾波器與上下限閾值比較器，數值超限時可自動產生中斷。
  * 專用於監控電源軌電壓（12V, 5V, 3.3V, 1.8V, Vcore 等）與類比溫度感測器。

### 5.9 PWM 輸出與風扇轉速計 (16-ch PWM / 16-ch Tach)
* **基底位址**：`0x1E78_7000`
* **控制能力**：
  * **16 組獨立硬體 PWM 控制通道**（可個別設定頻率與 Duty Cycle 0~100%）。
  * **16 組獨立 Tachometer 轉速計輸入通道**（硬體邊緣計數器，自動計算風扇 RPM 轉速）。
  * 支援風扇停轉異常偵測中斷與轉速超限警報。

### 5.10 通用輸入輸出 (GPIO & SGPIO, 228 pins)
* **基底位址**：`0x1E78_0000`
* **引腳分組**：
  * 228 支 GPIO，分為 Bank A ~ Bank AC（每 Bank 8 支）。
  * 支援 1.8V / 3.3V 電壓容限。
  * 每支 GPIO 可獨立配置：輸入/輸出、上拉/下拉、去彈跳濾波時間（Debounce Filter）、中斷觸發模式（上升邊緣、下降邊緣、雙邊緣、高/低電位）。
  * **GPIO Pass-Through Mode**：支援硬體直通模式（例如 Host Power Button 直通 PCH，BMC 可同時監聽或攔截）。
  * **SGPIO (Serial GPIO)**：支援 80 組串列式 GPIO 擴充介面（用於硬碟背板 LED 指示燈管理）。

### 5.11 2D 繪圖與視訊壓縮引擎 (Remote KVM Video Engine)
* **基底位址**：`0x1E70_0000`
* **圖形與 KVM 能力**：
  * 內建 2D 繪圖加速器，支援 PCIe VGA 顯示卡功能（最大解析度 1920×1200 @ 60Hz, 32bpp）。
  * **硬體視訊捕捉與壓縮引擎 (Video Engine)**：
    * 硬體即時捕捉 Host VGA 螢幕輸出畫面。
    * 支援 **動態差異區塊比對 (Differential Block Detection)**，僅壓縮有變化的畫面區塊。
    * 硬體 **JPEG / Aspeed Proprietary Video (ASR)** 壓縮，延遲低於 30ms，專用於 WebUI Remote KVM 串流。

### 5.12 硬體安全與 Root of Trust (OTP / Crypto / Secure Boot)
* **基底位址**：`0x1E6C_0000` (HACE)
* **安全特色**：
  * **Secure OTP (One-Time Programmable Memory)**：內建 8KB 安全一次性燒錄記憶體，存放 OEM Public Key Hash、AES Root Key、Secure Boot 鎖定旗標。
  * **硬體密碼加速器 (HACE)**：
    * 對稱加密：AES-128 / AES-192 / AES-256 (ECB, CBC, CTR, GCM)。
    * 非對稱密碼：RSA-2048 / RSA-3072 / RSA-4096、ECC (NIST P-256 / P-384 / ED25519)。
    * 雜湊運算：SHA-1, SHA-224, SHA-256, SHA-384, SHA-512, SHA-3, HMAC。
    * 真隨機數產生器 (TRNG)：符合 NIST SP 800-90B 規範。
  * **硬體安全開機流程 (Hardware Secure Boot)**：
    * ROM Code 由硬體 OTP Key 驗證 U-Boot SPL 簽章。
    * U-Boot SPL 驗證 U-Boot Proper。
    * U-Boot Proper 驗證 Linux FIT Image (Kernel + DTB + initramfs)。

### 5.13 看門狗計時器 (Watchdog Timer, 4 組)
* **基底位址**：`0x1E72_0000`
* **用途分工**：
  * **WDT1 / WDT2**：BMC 系統自身核心監控（死當自動重置 BMC）。
  * **WDT3**：Host 主機開機超時監控（BIOS POST Hang 自動重開機或切換備用 BIOS Flash）。
  * **WDT4**：安全開機還原監控（若更新失敗自動還原至 Golden Image）。

### 5.14 通用非同步收發器 (UART 1~5 + VUART + HUART)
* **基底位址**：`0x1E78_3000`
* **通訊配置**：
  * **UART5**：預設為 BMC Linux Debug Console (`ttyS4` @ 115200 bps)。
  * **UART1 ~ UART4**：可指派為 Host COM Port、外接模組通訊或 Serial-Over-LAN (SOL)。
  * **Virtual UART (VUART)**：Host 與 BMC 之間的虛擬串列埠，BIOS 可直接將開機訊息輸出至 VUART，由 BMC 轉為 IPMI SOL 或 SSH 終端。

### 5.15 USB 2.0 控制器 (Host + Virtual Device)
* **功能支援**：
  * **USB 2.0 Host 控制器** (EHCI / OHCI)：可外接 USB 儲存裝置、USB 網卡。
  * **USB 2.0 Device 控制器 (Virtual Hub)**：
    * **Virtual HID**：模擬 USB 鍵盤與滑鼠，供 Remote KVM 遠端操控。
    * **Virtual Mass Storage**：模擬 USB 隨身碟 / DVD-ROM（Virtual Media 功能，可透過網路掛載 ISO 開機安裝 OS）。
    * **CDC-Ether / RNDIS**：模擬 USB 虛擬網路卡，提供 Host ↔ BMC 高速內部乙太網路通訊。

---

## 6. 開發與實驗對照總表

| AST2600 硬體單元 | Linux 驅動模組 | OpenBMC 守護行程 | 對應實戰 Lab / 開發項目 |
| :--- | :--- | :--- | :--- |
| **I2C 匯流排 (16組)** | `i2c-aspeed.c` | `dbus-sensors` / `entity-manager` | **Lab 1-1** (EEPROM), **Lab 1-3** (TMP75) |
| **GPIO 控制器 (228p)** | `gpio-aspeed.c` | `phosphor-gpio-monitor` | **Lab 1-2** (LEDs), 過熱中斷監控 |
| **ADC 轉換器 (16ch)** | `aspeed_adc.c` | `adcsensor` | **Lab 3-2** (電壓監控與閾值警報) |
| **PWM & Tachometer** | `aspeed_pwm_tacho.c`| `fansensor` / `phosphor-pid-control` | **Lab 3-3** (風扇 PID 轉速策略) |
| **LPC / eSPI (MBOX/KCS)**| `kcs_aspeed.c` | `pldmd` / `phosphor-ipmi-host` | **Lab 2-1**, **Lab 2-2** (BIOS 溝通) |
| **PCIe VDM / MCTP** | `mctp-serial` / `af_mctp` | `mctpd` / `pldmd` | **Lab 2-3** (PLDM FW Update) |
| **FMC / SPI-SMC** | `spi-aspeed-smc.c` | `phosphor-software-manager` | BIOS SPI Flash 更新與 Dual-Flash 切換 |
| **Snoop (Port 80h)** | `aspeed_lpc_snoop.c`| `postcode-manager` | Host 開機 POST Code 監控與記錄 |
| **Video Engine** | `aspeed-video.c` | `bmcweb` (WebSocket KVM) | HTML5 遠端視訊桌面串流 |
| **USB Virtual Device** | `aspeed-vhub.c` | `jsnbd` / `bmcweb` | 虛擬光碟機 (Virtual Media ISO 掛載) |
| **Secure Boot / OTP** | `aspeed-otp.c` | `otptool` / `socsec` | 安全開機簽章驗證與金鑰燒錄 |

---

*本文件由 ONYX 韌體架構組編撰，供架構師設計新硬體平台與規劃後續功能實驗參考。*
