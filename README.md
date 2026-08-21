# meta-onyx

ONYX Healthcare vendor BSP layer for OpenBMC.

## Description

This layer provides ONYX Healthcare-specific customizations for the
AST2600 platform, including:

- **onyx-ipmi**: OEM IPMI commands under NetFn 0x30 (GetVersion, AddSub, Echo)
- **onyx-service**: D-Bus service exposing OEM properties for Redfish
- **GPIO monitor**: Custom GPIO pin configuration for ONYX hardware
- **bmcweb Redfish**: OEM Redfish endpoint at `/redfish/v1/Oem/Onyx/Info`

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
