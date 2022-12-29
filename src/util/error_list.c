/*
 * errormsg.c - functions used in all phases of the compiler to give
 *              error messages about the Tiger program.
 *
 */

#include "error_list.h"
#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct error_msg error_msg;

struct error_msg {
    int line;
    int pos;
    char *msg;
    error_msg *next;
};

static error_msg *head = NULL;
static error_msg *tail = NULL;
static const char *fileName = "(no name)";

void error_list_init(const char *file_name) { fileName = file_name; }

void error_list_add(int line, int pos, const char *msg, ...)
{
    va_list args;
    error_msg *error;
    char args_buf[BUFSIZ];

    /* Create formatted string in buffer */
    va_start(args, msg);
    vsnprintf(args_buf, BUFSIZ, msg, args);

    /* Create error msg */
    error = xmalloc(sizeof(error_msg));
    error->line = line;
    error->pos = pos;
    error->msg = strdup(args_buf);
    error->next = NULL;

    /* Append to list */
    if (head == NULL) {
        head = error;
        tail = error;
    } else {
        tail->next = error;
        tail = error;
    }
}

void error_list_free()
{
    error_msg *next;

    while (head != NULL) {
        next = head->next;
        free(head->msg);
        free(head);
        head = next;
    }
}

void error_list_print(FILE *out, bool file_name)
{
    error_msg *list;

    for (list = head; list != NULL; list = list->next) {
        if (file_name) {
            fprintf(out, "%s: %d.%d: %s\n", fileName, list->line, list->pos,
                    list->msg);
        } else {
            fprintf(out, "%d.%d: %s\n", list->line, list->pos, list->msg);
        }
    }
}

bool error_list_empty() { return head == NULL; }