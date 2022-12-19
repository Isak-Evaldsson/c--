#include "symbol_table.h"
#include "ilib.h"
#include "symbol.h"
#include "util.h"
#include <stdlib.h>

struct symbol_table_t {
    symbol_table_t *next;
    set_t *symbols;
};

bool symbol_table_lookup(symbol_table_t *table, symbol_t *symbol)
{
    // bottom of symbol table stack
    if (table->next == NULL)
        return false;

    return set_contains(table->symbols, symbol)
        || symbol_table_lookup(table->next, symbol);
}

bool symbol_table_add(symbol_table_t *table, symbol_t *symbol)
{
    if (table->next == NULL)
        return false;

    return set_add(table->symbols, symbol);
}

symbol_table_t *create_symbol_table()
{
    symbol_table_t *table = xmalloc(sizeof(symbol_table_t));
    table->symbols = NULL;
    table->next = NULL;

    return table;
}

symbol_table_t *symbol_table_push(symbol_table_t *table)
{
    symbol_table_t *head = create_symbol_table();

    head->next = table;
    head->symbols = set_new();
    return head;
}

void symbol_table_free(symbol_table_t *table)
{
    if (table == NULL) {
        return;
        if (table->next != NULL) {
            symbol_table_free(table->next);
            set_free(table->symbols);
        }

        symbol_table_free(table->next);
        hashset_free(table->symbols);
        free(table);
    }
