/*
    Symbol implementation. To maximite efficiency, all the symbols are stored
    within an global hash table.
*/
#include "symbol.h"
#include "util.h"
#include <stddef.h>
#include <string.h>

#define SIZE 100

struct symbol_t {
    char *name;
    symbol_t *next; // allows chaining
};

// TODO: make hashtable array able to scale
static symbol_t *hashtable[SIZE];

static symbol_t *alloc_symbol(char *string, symbol_t *next)
{
    symbol_t *sym = xmalloc(sizeof(symbol_t));
    sym->name = string;
    sym->next = next;
    return sym;
}

static void free_symbol(symbol_t *s)
{
    if (s == NULL)
        return;

    free_symbol(s->next);
    free(s->name);
    free(s);
}

static unsigned int hash(char *s0)
{
    unsigned int h = 0;
    char *s;
    for (s = s0; *s; s++)
        h = h * 65599 + *s;
    return h;
}

symbol_t *get_symbol(char *name)
{
    int index = hash(name) % SIZE;
    symbol_t *head = hashtable[index], *sym;

    for (sym = head; sym != NULL; sym = sym->next) {
        if (strcmp(name, sym->name) == 0) {
            // makes sure that duplicated strings are properly free'd
            free(name);

            return sym;
        }
    }

    sym = alloc_symbol(name, head);
    hashtable[index] = sym;
    return sym;
}

char *symbol_name(symbol_t *sym) { return sym->name; }

void free_symbols()
{
    // Free invidual symbols
    for (size_t i = 0; i < SIZE; i++) {
        symbol_t *sym = hashtable[i];

        if (sym != NULL) {
            free_symbol(sym);
        }
    }

    // Clear table
    memset(hashtable, 0, SIZE * sizeof(hashtable[0]));
}
