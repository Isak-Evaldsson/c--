#include "hashset.h"
#include "symbol.h"

typedef struct symboltable_t symboltable_t;

struct symboltable_t {
    symboltable_t *next;
    hashset_t *symbols;
};

bool symboltable_lookup(symboltable_t *table, symbol_t *symbol)
{
    // bottom of symboltable stack
    if (table->next == NULL)
        return false;

    return hashset_contains(table->symbols, symbol)
        || symboltable_lookup(table->next, symbol);
}

bool symboltable_declare(symboltable_t *table, symbol_t *symbol)
{
    if (table->next == NULL)
        return false;

    return hashset_add(table->symbols, symbol);
}

symboltable_t *create_symboltable()
{
    symboltable_t *table = xmalloc(sizeof(symboltable_t));
    table->symbols = NULL;
    table->next = NULL;

    return table;
}

void symboltable_push(symboltable_t **table)
{
    symboltable_t *head = create_symboltable();

    head->next = *table;
    head->symbols = create_hashset();
    *table = head;
}

void symboltable_free(symbol_t *table)
{
    if (table->next != NULL) {
        symboltable_free(table->next);
        hashset_free(table->name);
    }
    free(table);
}
