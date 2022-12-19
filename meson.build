project('mycompiler', 'c',
        default_options : ['c_std=c17'])

cc = meson.get_compiler('c')

# src/include collecting all the src files
src = []
inc = []

# Adding I-Lib dependency
inc += include_directories('./I-Lib/include')
ilib_dir = meson.current_source_dir() / 'I-Lib/build'
ilib_dep = cc.find_library('i', dirs : ilib_dir)

# subdirs adding src files
subdir('src/ast')
subdir('src/util')
subdir('src/lexer')
subdir('src/parser')
subdir('src/semantic_analysis')

executable('compiler',
    sources: [src, 'src/compiler.c'],
    include_directories: inc,
)

# unit tests
test_bin = executable('tests', 
    sources: ['tests/test_framework.c', 'tests/main.c', src],
    include_directories: inc,
    c_args: [
        '-g',
    ],
    dependencies: ilib_dep,
)

test('unit tests', test_bin, args: meson.current_source_dir() / 'tests')