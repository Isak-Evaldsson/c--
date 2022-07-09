project('mycompiler', 'c',
        default_options : ['c_std=c17'])

# Defining lexer and parser generators
flex  = find_program('flex')
bison = find_program('bison')

lexer = generator(flex,
                  output : '@PLAINNAME@.yy.c',
                  arguments : ['-o', '@OUTPUT@', '@INPUT@'])

parser = generator(bison,
                   output : ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
                   arguments : ['@INPUT@',
                                '--defines=@OUTPUT1@',
                                '--output=@OUTPUT0@',
                                '-Wcounterexamples'])

# Defining src files
src = [
    'src/errormsg.c',
    'src/util.c',
    'src/ast.c',
    'src/parser.c'
]

# Adding lexer/parser generator output
src += lexer.process('src/scanner.flex')
src += parser.process('src/parser.y')

executable('compiler',
    sources: [src, 'src/compiler.c']
)

# unit tests
test_bin = executable('tests', 
    sources: ['tests/parse_tester.c', src],
    include_directories: 'src'
)

test('unit tests', test_bin, args: meson.current_source_dir() / 'tests/files/parser')