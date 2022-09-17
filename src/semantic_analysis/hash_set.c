/*
    Generic hash table implementation using chaining
*/
#include "hash_set.h"
#include "util.h"

#define INITIAL_SIZE (10)

/* Internal structure holding the table chains */
typedef struct bucket_t bucket_t;

struct bucket_t {
    void *value;
    bucket_t *next;
};

/* Hash table data structure */
struct hashset_t {
    unsigned int size;
    unsigned int entries; /* allows table to grow when its to well-filled */
    bucket_t **array
};

/* creates new bucket and appends it to the next */
static bucket_t *create_bucket(void *value, bucket_t *next)
{
    bucket_t *b = xmalloc(sizeof(bucket_t));
    b->value = value;
    b->next = next;
    return b;
}

static void free_bucket(bucket_t *bucket)
{
    if (bucket == NULL)
        return;

    if (bucket->next != NULL) {
        free_bucket(bucket->next);
    }
    free(bucket);
}

static void append_bucket(bucket_t *list, bucket_t *elem)
{
    bucket_t *b;

    // find last elem in list, assumes list is not null
    for (b = list; b->next != NULL; b = b->next)
        ;

    b->next = elem;
}

/* expand tables, returns 1 if successful */
static void expand_table(hashset_t *set)
{
    unsigned int new_size = 2 * set->size;
    bucket_t **old_array = set->array;
    bucket_t **new_array = xcalloc(new_size, sizeof(bucket_t));

    // re-balance table
    for (unsigned int i = 0; i < set->size; i++) {
        unsigned int new_index;
        bucket_t *b = old_array[i];
        bucket_t *prev;

        if (b != NULL) {
            new_index = (int)b % new_size;
            prev = new_array[new_index];

            // if prev list exits, prepend the new one
            if (prev != NULL) {
                append_bucket(b, prev);
            }
            new_array[new_index] = b;
        }
    }

    // assign the new table
    set->array = new_array;
    set->size = new_size;

    // clean-up
    free(old_array);
}

hashset_t *create_hashset()
{
    hashset_t *set = xmalloc(sizeof(hashset_t));
    bucket_t **array = xcalloc(sizeof(bucket_t *), INITIAL_SIZE);

    set->size = INITIAL_SIZE;
    set->entries = 0;
    set->array = array;

    return set;
};

bool hashset_add(hashset_t *set, void *value)
{
    int index = (int)value % set->size;
    bucket_t *head = set->array + index;

    // check if element already exits
    for (bucket_t *b = head; b != NULL; b = b->next) {
        if (b->value == value)
            return false;
    }

    set->array[index] = create_bucket(value, head);
    set->entries++;

    if (set->entries / set->size > 2) {
        expand_table(set);
    }

    return true;
}

bool hashset_contains(hashset_t *set, void *value)
{
    bucket_t *b;
    int index = (int)value % set->size;

    for (b = set->array[index]; b != NULL; b = b->next) {
        if (b->value == value)
            return true;
    }
    return false;
}

void hashset_free(hashset_t *set)
{
    // frees buckets
    for (unsigned int i = 0; i < set->size; i++) {
        free_bucket(set->array + i);
    }
    free(set);
}