/*
 * errormsg.c - functions used in all phases of the compiler to give
 *              error messages about the Tiger program.
 *
 */

#include "errormsg.h"
#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

bool anyErrors = false;

static char *fileName = "";
static int lineNum = 1;

int EM_tokPos = 0;

extern FILE *yyin;

typedef struct intList {
    int i;
    struct intList *rest;
} IntList;

static IntList *intList(int i, IntList *rest)
{
    IntList *l = xmalloc(sizeof *l);
    l->i = i;
    l->rest = rest;
    return l;
}

static void freeList(IntList *list)
{
    if (list->rest)
        freeList(list->rest);
    free(list);
}

static IntList *linePos = NULL;

void EM_newline(void)
{
    lineNum++;
    linePos = intList(EM_tokPos, linePos);
}

void EM_error(int pos, char *message, ...)
{
    va_list ap;
    IntList *lines = linePos;
    int num = lineNum;

    anyErrors = true;
    while (lines && lines->i >= pos) {
        lines = lines->rest;
        num--;
    }

    if (fileName)
        fprintf(stderr, "%s:", fileName);
    if (lines)
        fprintf(stderr, "%d.%d: ", num, pos - lines->i);
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}

void EM_reset(char *fname)
{
    anyErrors = false;
    fileName = fname;
    lineNum = 1;
    linePos = intList(0, NULL);
    yyin = fopen(fname, "r");
    if (!yyin) {
        EM_error(0, "cannot open");
        exit(1);
    }
}

void EM_free()
{
    if (linePos)
        freeList(linePos);
}