#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

void *xmalloc(size_t s);
void error(const char *fmt, ...);

// Custom implementation of the strdup function proposed in c2x
char *strdup(const char *src);

typedef struct bool_list bool_list;
struct bool_list {
    bool value;
    bool_list *next;
};

bool_list *bool_list_create(bool value, bool_list *next);