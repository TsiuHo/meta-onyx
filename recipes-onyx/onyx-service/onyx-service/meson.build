# SPDX-License-Identifier: Apache-2.0

project(
    'onyx-service',
    'cpp',
    version: '1.0.1',
    meson_version: '>=1.1.1',
    default_options: [
        'cpp_std=c++23',
        'warning_level=3',
        'werror=false',
    ],
)

# ─────────────────────────────────────────────────
# Dependencies
# ─────────────────────────────────────────────────
sdbusplus_dep = dependency('sdbusplus')
boost_dep = dependency('boost')

# ─────────────────────────────────────────────────
# Build the daemon executable
# ─────────────────────────────────────────────────
executable(
    'onyx-service',
    'src/main.cpp',
    dependencies: [
        sdbusplus_dep,
        boost_dep,
    ],
    install: true,
    install_dir: get_option('bindir'),
)

# ─────────────────────────────────────────────────
# Install systemd service file
# ─────────────────────────────────────────────────
systemd = dependency('systemd', required: true)
systemd_system_unit_dir = systemd.get_variable(
    'systemdsystemunitdir',
    pkgconfig_define: ['prefix', get_option('prefix')]
)

install_data(
    'service/com.onyx.OemInfo.service',
    install_dir: systemd_system_unit_dir,
)
