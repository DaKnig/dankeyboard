project('dankeyboard', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

gtkdep = dependency('gtk4')

exe = executable('dankeyboard', 'src/dankeyboard.c',
                 dependencies: gtkdep, install : true)

test('basic', exe)
