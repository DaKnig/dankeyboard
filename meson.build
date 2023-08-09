project('dankeyboard', 'c',
  version : '0.1',
  default_options : ['c_std=gnu2x', 'warning_level=3'])

gtkdep = dependency('gtk4')
adwdep = dependency('libadwaita-1')

exe = executable('dankeyboard', 'src/dankeyboard.c',
                 dependencies: [gtkdep, adwdep], install : true)

test('basic', exe)
