/*
    Global error message list used throughout the compiler
*/
#ifndef ERROR_LIST_H
#define ERROR_LIST_H

#include <stdbool.h>

void error_list_add(int line, int pos, const char *msg, ...);
void error_list_init(const char *file_name);
void error_list_print();
void error_list_free();
bool error_list_empty();

#endif /* ERROR_LIST_H */