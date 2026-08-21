# SPDX-License-Identifier: Apache-2.0

project(
    'onyx-ipmi',
    'cpp',
    version: '1.0.1',
    meson_version: '>=1.1.1',
    default_options: [
        'cpp_std=c++23',
        'warning_level=3',
        'werror=false',
    ],
)

# ─────────────────────────────────────────────────────────────
# Dependencies
# ─────────────────────────────────────────────────────────────
# libipmid (provided by phosphor-ipmi-host) exports ipmid/api.hpp and ipmi::registerHandler
ipmid_dep = dependency('libipmid')
# phosphor-logging provides structured logging
logging_dep = dependency('phosphor-logging')

# ─────────────────────────────────────────────────────────────
# Build shared library (IPMI provider plugin)
# ─────────────────────────────────────────────────────────────
onyx_ipmi_lib = shared_library(
    'onyxoemcmds',
    'src/onyx-ipmi.cpp',
    version: meson.project_version(),
    dependencies: [
        ipmid_dep,
        logging_dep,
    ],
    install: true,
    install_dir: get_option('libdir') / 'ipmid-providers',
    override_options: ['b_lundef=false'],
)
