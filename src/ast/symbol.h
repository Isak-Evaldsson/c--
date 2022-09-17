/*
    Symbols replacing the parsed strings. If the same string is parsed multiple
    times, the same symbol pointer will be used, thereby making symbol
    management easier and more efficient throughout the compiler
*/
#ifndef SYMBOL_HEADER
#define SYMBOL_HEADER

typedef struct symbol_t symbol_t;

// Gets the appropiate symbol form the symbol table
symbol_t *get_symbol(char *name);

// Return the name associated to a symbol
char *symbol_name(symbol_t *sym);

#endif
