project(
  'fsat2-engine',
  'c',
  version: '1.3',
  default_options: [
    'warning_level=3',
    'c_std=gnu11',
    'optimization=3',
    'buildtype=release',
  ],
)

fsat2_engine_srcs = []

fsat2_engine_inc = include_directories('src')

fsat2_engine_deps = []

zmq = dependency('libzmq')

fsat2_engine_deps += zmq

c_args = [
  '-Wno-unused-parameter',
  '-Wno-unused-value',
  '-Wno-missing-field-initializers',
  '-Wno-narrowing',
  '-Wno-pointer-arith',
  '-Wshadow',
  '-Wcast-align',
  '-Wwrite-strings',
]

subdir('src')

fsat2_engine = executable(
  meson.project_name(),
  sources: fsat2_engine_srcs,
  include_directories: fsat2_engine_inc,
  dependencies: fsat2_engine_deps,
  c_args: c_args,
  install: true,
)

install_data('services/fsat2-engine.service',
             install_dir: get_option('systemd_system_unitdir'),
             rename: 'fsat2-engine.service')
