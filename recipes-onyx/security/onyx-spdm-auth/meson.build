project(
    'onyx-spdm-auth',
    'cpp',
    version: '1.0',
    default_options: [
        'warning_level=3',
        'cpp_std=c++23'
    ])

sdbusplus = dependency('sdbusplus')
phosphor_logging = dependency('phosphor-logging')

executable(
    'onyx-spdm-auth',
    'src/onyx-spdm-auth.cpp',
    dependencies: [sdbusplus, phosphor_logging],
    install: true,
    install_dir: get_option('bindir')
)
