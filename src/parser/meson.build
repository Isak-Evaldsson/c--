# Add folder to include dir
inc += include_directories('.')

# Defining parser generator
bison = find_program('bison')

parser = generator(bison,
                   output : ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
                   arguments : ['@INPUT@',
                                '--defines=@OUTPUT1@',
                                '--output=@OUTPUT0@',
                                '-Wcounterexamples'])

# Adding parser generator output
src += parser.process('parser.y')
src += files('parser.c')