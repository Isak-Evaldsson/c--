/*
    Generic hash table implementation
*/
#include <stdbool.h>
#include <stddef.h>

typedef struct hashset_t hashset_t;

/* Constructs table */
hashset_t *create_hashset();

/* Adds a new value to the set, returns false if value already exits */
bool hashset_add(hashset_t *set, void *value);

/* Gets the value associated with the specified key */
bool hashset_contains(hashset_t *set, void *value);

/* Frees hashset */
void hashset_free(hashset_t *set);