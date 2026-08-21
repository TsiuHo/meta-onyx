project('onyx-pldm-bios', 'cpp',
    version: '1.0',
    default_options: ['cpp_std=c++23'])

sdbusplus_dep = dependency('sdbusplus')
phosphor_logging_dep = dependency('phosphor-logging')

executable('onyx-pldm-bios',
    'src/onyx-pldm-bios.cpp',
    dependencies: [sdbusplus_dep, phosphor_logging_dep],
    install: true)
