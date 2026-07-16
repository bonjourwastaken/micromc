project(
  'micromc',
  'c',
  version: '0.1',
  default_options: [
    'warning_level=3',
    'prefer_static=true',
    'b_ndebug=if-release',
    'b_vscrt=static_from_buildtype',
    'c_std=c23',
    'optimization=1'
  ],
)

if get_option('buildtype') == 'release'
  add_project_link_arguments('-s', language: 'c')
endif

cmake = import('cmake')
glfw_opt = cmake.subproject_options()
glfw_opt.add_cmake_defines(
  {
    'GLFW_BUILD_EXAMPLES': false,
    'GLFW_BUILD_TESTS': false,
    'GLFW_BUILD_DOCS': false,
    'GLFW_INSTALL': false,
    'GLFW_BUILD_X11': false,
  },
)
glfw_opt.set_override_option('warning_level', '0')
glfw = cmake.subproject('glfw', options: glfw_opt).dependency('glfw')
cglm = subproject('cglm', default_options: ['warning_level=0']).get_variable('cglm_dep')
math = meson.get_compiler('c').find_library('m', required: false)

src = ['src/main.c', 'src/renderer.c', 'src/world.c', 'src/gl.c']
inc = [include_directories('inc')]
dep = [glfw, cglm, math]

executable(
  meson.project_name(),
  src,
  include_directories: inc,
  dependencies: dep,
  win_subsystem: 'windows',
)
