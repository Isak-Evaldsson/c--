# Defining lexer generators
flex  = find_program('flex')

lexer = generator(flex,
                  output : '@PLAINNAME@.yy.c',
                  arguments : ['-o', '@OUTPUT@', '@INPUT@'])

# Adding lexer generator output
src += lexer.process('scanner.flex')
