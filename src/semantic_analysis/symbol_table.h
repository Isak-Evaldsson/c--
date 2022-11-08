#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "symbol.h"
#include <stdbool.h>

typedef struct symbol_table_t symbol_table_t;

symbol_table_t *create_symbol_table();
symbol_table_t *symbol_table_push(symbol_table_t *table);
bool symbol_table_lookup(symbol_table_t *table, symbol_t *symbol);
bool symbol_table_add(symbol_table_t *table, symbol_t *symbol);
void symbol_table_free(symbol_table_t *table);

#endif
