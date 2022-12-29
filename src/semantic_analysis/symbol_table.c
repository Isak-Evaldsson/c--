#include "symbol_table.h"
#include "ilib.h"
#include "symbol.h"
#include "util.h"
#include <stdlib.h>

struct symbol_table_t {
    symbol_table_t *next;
    set_t *symbols;
    vector_t *children;
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
    table->children = NULL;

    return table;
}

symbol_table_t *symbol_table_push(symbol_table_t *table)
{
    symbol_table_t *head = create_symbol_table();

    // Add child node to parent
    if (table->children == NULL) {
        table->children = vector_new();
    }

    vector_push(table->children, head);

    head->next = table;
    head->symbols = set_new();
    return head;
}

void symbol_table_free(symbol_table_t *table)
{
    size_t n_children = 0;
    symbol_table_t **children;

    if (table == NULL) {
        return;
    }

    // Free children if it exits
    if (table->children != NULL) {
        children = vector_array_ptr(table->children, &n_children);

        for (size_t i = 0; i < n_children; i++) {
            symbol_table_free(children[i]);
        }
        vector_free(table->children);
    }

    set_free(table->symbols);
    free(table);
}
