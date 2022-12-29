/*
    Generic double linked list implementation
*/
#ifndef LIST_H
#define LIST_H

typedef struct list_t list_t;

struct list_t {
    void *data;
    list_t *prev;
    list_t *next;
};

// Creates a linked list
list_t *create_list(void *data);

// Adds data to end of list
void append(list_t **list, void *data);

// Frees the linked list (not its data)
void free_list(list_t **list);

// Frees the linked list, including its data using the supplied free_func
void free_list_data(list_t **list, void(free_func)(void *));

#endif
