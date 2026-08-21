project('onyx-rot-manager', 'cpp',
    version: '1.0',
    default_options: ['cpp_std=c++23'])

sdbusplus_dep = dependency('sdbusplus')
phosphor_logging_dep = dependency('phosphor-logging')
boost_dep = dependency('boost')

executable('onyx-rot-manager',
    'src/onyx-rot-manager.cpp',
    dependencies: [sdbusplus_dep, phosphor_logging_dep, boost_dep],
    install: true)

install_data('service/xyz.openbmc_project.Control.Security.RoT.service',
    install_dir: '/usr/lib/systemd/system')
