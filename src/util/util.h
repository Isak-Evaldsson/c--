#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// Allocates memory exits program if allocation fails
void *xmalloc(size_t s);
void *xcalloc(size_t s, size_t n);

// Prints message to stderr and exits program
void error(const char *fmt, ...);

// Custom implementation of the strdup function proposed in c2x
char *strdup(const char *src);

typedef struct bool_list bool_list;
struct bool_list {
    bool value;
    bool_list *next;
};

bool_list *bool_list_create(bool value, bool_list *next);