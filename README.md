# meta-onyx

ONYX Healthcare vendor BSP layer for OpenBMC.

## Description

This layer provides ONYX Healthcare-specific customizations, services, and hardware configurations for the AST2600 platform. The current architecture includes:

### 1. ONYX Core Services (`recipes-onyx`)
- **Sensors & Thermal**: `onyx-fan-control`, `onyx-sensor-monitor`, `onyx-voltage-monitor`
- **Security & SPDM**: `onyx-rot-manager`, `onyx-spdm-auth`, `onyx-spdm-attestation`, `onyx-pcie-telemetry`
- **PLDM Stack**: Comprehensive PLDM support including BIOS, FRU, RDE, Thermal, and Firmware Update (`onyx-pldm-*`)
- **MCTP & Network**: `onyx-secure-mctp`, `onyx-smbus-mctp`, `onyx-ncsi-manager`
- **System Management**: `onyx-service`, `onyx-power-wdt` (Watchdog), `onyx-postcode-snoop`
- **Virtual Media**: `onyx-vmedia-manager`

### 2. OpenBMC Phosphor Overrides (`recipes-phosphor`)
- **bmcweb**: OEM Redfish endpoint patches (e.g., `/redfish/v1/Oem/Onyx/...`) and custom Fan Control endpoints.
- **Entity Manager**: Hardware topologies and JSON configurations for ONYX lab baseboards.
- **GPIO & Virtual Sensors**: Customized configurations for `phosphor-gpio-monitor` and `phosphor-virtual-sensor`.

---

## Download & Setup

### For Colleagues (Download)
To download and add this layer to your OpenBMC environment, simply clone this repository into your workspace:

```bash
cd /home/hao/OpenBMC
git clone https://github.com/TsiuHo/meta-onyx.git
```

### For Developers (Push Updates)
If you make changes to this layer, remember to commit and push from within the `meta-onyx` directory:

```bash
cd /home/hao/OpenBMC/meta-onyx
git commit -a -m "Update descriptions"
git push origin main
```

*(Note: If this layer is added as a submodule in the main OpenBMC repository, remember to update the pointer in the main repository as well.)*

---

## Dependencies

This layer depends on:

- meta-phosphor
- meta-aspeed
- meta-openembedded (meta-oe, meta-networking, meta-python)

## Machine

- `onyx-ast2600`: ONYX AST2600-based BMC platform

## Build

```bash
. setup onyx-ast2600
bitbake obmc-phosphor-image
```

## Maintainer

- HaoChou <HaoChou@onyx-healthcare.com>
