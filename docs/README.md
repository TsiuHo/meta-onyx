# ONYX OpenBMC 實體主機板韌體架構與開發手冊

本文件為 **ONYX Healthcare OpenBMC (AST2600 平台)** 專用的量產與硬體對接開發手冊，涵蓋 **GPIO 控制**、**OEM IPMI 指令**、**Redfish OEM 端點**、**PLDM/MCTP 驅動**、**eSPI Snoop 監控**、**硬體安全開機** 與 **PCIe VDM 遙測**。

---

## 📚 專案核心技術文件索引

所有架構與硬體對接文件均位於 `meta-onyx/docs/`：

| 文件名稱 | 類型 | 說明 |
| :--- | :---: | :--- |
| [**developer-handbook.md**](file:///d:/openbmc/meta-onyx/docs/developer-handbook.md) | 📖 必讀 | **跨平台韌體架構師完整開發手冊**（接手 RD 首要參考指南） |
| [**ast2600-hardware-datasheet-reference.md**](file:///d:/openbmc/meta-onyx/docs/ast2600-hardware-datasheet-reference.md) | 📑 規格 | **AST2600 硬體規格與晶片架構手冊**（SoC、記憶體映射、各 IP 暫存器） |
| [**01-dc-scm-hardware-architecture.md**](file:///d:/openbmc/meta-onyx/docs/01-dc-scm-hardware-architecture.md) | 🔌 硬體 | **DC-SCM 硬體架構**（Device Tree、I2C EEPROM、GPIO LED、hwmon） |
| [**02-mctp-pldm-bios-firmware-guide.md**](file:///d:/openbmc/meta-onyx/docs/02-mctp-pldm-bios-firmware-guide.md) | 🔌 硬體 | **MCTP/PLDM BIOS 與韌體更新**（DSP0247/DSP0267、/dev/aspeed-kcs3、/dev/mtd/bios） |
| [**03-redfish-sensor-voltage-fan-guide.md**](file:///d:/openbmc/meta-onyx/docs/03-redfish-sensor-voltage-fan-guide.md) | 🔌 硬體 | **Sensor 資料流與風扇控制**（hwmon sysfs、SAR ADC 分壓換算、PWM 閉迴路） |
| [**04-espi-lpc-postcode-snoop-guide.md**](file:///d:/openbmc/meta-onyx/docs/04-espi-lpc-postcode-snoop-guide.md) | 🔌 硬體 | **eSPI/LPC Snoop 開機碼監控**（/dev/aspeed-lpc-snoop0、Port 80h/81h、Hang 超時） |
| [**05-hardware-root-of-trust-secure-boot.md**](file:///d:/openbmc/meta-onyx/docs/05-hardware-root-of-trust-secure-boot.md) | 🔌 硬體 | **硬體信任根與安全開機**（AST2600 OTP 8KB eFuse、HACE 加密引擎 /dev/crypto） |
| [**06-video-engine-kvm-virtual-media.md**](file:///d:/openbmc/meta-onyx/docs/06-video-engine-kvm-virtual-media.md) | 🔌 硬體 | **Video Engine KVM 與虛擬媒體**（/dev/video0、aspeed-vhub USB Gadget ConfigFS） |
| [**07-mipi-i3c-controller-integration.md**](file:///d:/openbmc/meta-onyx/docs/07-mipi-i3c-controller-integration.md) | 🔌 硬體 | **MIPI I3C 主控制器**（/dev/i3c-0、CCC ENTDAA 動態位址、IBI 帶內中斷） |
| [**08-ncsi-sideband-network-architecture.md**](file:///d:/openbmc/meta-onyx/docs/08-ncsi-sideband-network-architecture.md) | 🔌 硬體 | **Gigabit MAC 與 NC-SI 旁路網路**（Generic Netlink NC-SI 模組、多通道容錯） |
| [**09-power-sequencing-watchdog-guide.md**](file:///d:/openbmc/meta-onyx/docs/09-power-sequencing-watchdog-guide.md) | 🔌 硬體 | **x86 電源時序與硬體看門狗**（libgpiod、/dev/watchdog1、Dual-Flash SPI 切換） |
| [**10-pcie-vdm-mctp-telemetry-guide.md**](file:///d:/openbmc/meta-onyx/docs/10-pcie-vdm-mctp-telemetry-guide.md) | 🔌 硬體 | **PCIe Gen2 Endpoint 與 MCTP VDM**（/dev/aspeed-pcie-vdm、64-byte TLP DMA 遙測） |
| [**redfish-oem-development-guide.md**](file:///d:/openbmc/meta-onyx/docs/redfish-oem-development-guide.md) | 🛠️ 指南 | **Redfish OEM 端點開發範本**（GET / PATCH / POST 模式教學） |

---

## 1. 專案目錄結構

```text
d:\openbmc\meta-onyx├── conf/
│   ├── layer.conf                                 # Layer 註冊設定
│   ├── machine/
│   │   └── onyx-ast2600.conf                      # ONYX 專用 Machine 設定
├── docs/                                          # 完整硬體規格與驅動架構手冊
├── recipes-onyx/
│   ├── ipmi/                                      # ONYX OEM IPMI 指令 (NetFn 0x30)
│   ├── onyx-service/                              # ONYX OEM D-Bus / Redfish 服務
│   ├── dts/                                       # DC-SCM DTS 覆蓋層 (EEPROM, LED, TMP421)
│   ├── pldm/                                      # PLDM BIOS 與 MTD 韌體更新 Daemon
│   ├── sensors/                                   # hwmon 遙測、SAR ADC 電壓與 PWM 風扇控制器
│   ├── postcode/                                  # eSPI/LPC Snoop POST Code 監控服務
│   ├── security/                                  # AST2600 OTP 與硬體信任根 (RoT)
│   ├── vmedia/                                    # V4L2 Video 與 USB Gadget 虛擬媒體
│   ├── i3c/                                       # MIPI I3C 主控制器與 DAA 列舉
│   ├── network/                                   # NC-SI 1.1 Netlink 網路管理
│   ├── power/                                     # x86 電源時序與硬體看門狗 (/dev/watchdog1)
│   └── mctp/                                      # PCIe Gen2 Endpoint MCTP VDM 遙測
└── recipes-phosphor/
    ├── configuration/entity-manager/              # Entity-Manager 自動發現設定
    ├── gpio/                                      # GPIO 監控配置 (phosphor-gpio-monitor)
    ├── images/                                    # Image 打包 (obmc-phosphor-image)
    ├── interfaces/bmcweb/                         # bmcweb OEM Redfish patches
    └── sensors/phosphor-virtual-sensor/           # 虛擬感測器公式設定
```

---

## 2. Build 指令

```bash
# 初始化建置環境 (使用 onyx-ast2600 machine)
. setup onyx-ast2600

# 編譯完整量產 Image
bitbake obmc-phosphor-image
```
