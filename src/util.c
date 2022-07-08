/*
 * util.c - commonly used utility functions.
 */

#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZ];

    va_start(args, fmt);
    vsnprintf(buf, BUFSIZ, fmt, args);

    fprintf(stderr, "error: %s\n", buf);
    exit(1);
}

void *xmalloc(size_t s)
{
    void *p = malloc(s);

    if (p == NULL)
        error("out of memory: malloc(%zu) failed", s);

    return p;
}

char *strdup(const char *src)
{
// Making sure the strdup function wont compile if c2x or above
#if __STDC_VERSION > 201710L
#error("replace strdup implementation with the libc version")
#endif
    size_t len = strlen(src) + 1;
    char *str = malloc(len);
    strncpy(str, src, len);
    return str;
}

bool_list *bool_list_create(bool value, bool_list *next)
{
    bool_list *list = xmalloc(sizeof(*next));
    list->value = value;
    list->next = next;
    return list;
}