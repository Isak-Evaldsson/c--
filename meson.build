project('mycompiler', 'c',
        default_options : ['c_std=c17'])

# src/include collecting all the src files
src = []
inc = []

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
    sources: ['tests/parse_tester.c', src],
    include_directories: inc,
    c_args: [
        '-fsanitize-recover=address',
        '-fsanitize=address',
        '-g'
    ],
    link_args: ['-lasan']
)

test('unit tests', test_bin, args: meson.current_source_dir() / 'tests/files/parser')