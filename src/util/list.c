#include "list.h"
#include "util.h"

list_t *create_list(void *data)
{
    list_t *list = xmalloc(sizeof(list));
    list->next = list->prev = list;
    list->data = data;

    return list;
}

void append(list_t **list, void *data)
{
    list_t *tmp, *head = *list, *new_list = create_list(data);

    if (head == NULL) {
        *list = new_list;
    } else {
        // Append new_list to end of tmp
        tmp = head->prev;
        tmp->next = new_list;

        // Set new list in between tmp and head
        new_list->next = head;
        new_list->prev = tmp;

        // Fixed circularity
        head->prev = new_list;
    }
}

// Frees the linked list (not its data)
void free_list(list_t **list)
{
    list_t *head, *tmp;

    head = tmp = *list;

    do {
        tmp = tmp->next;

    } while (head != tmp);
}

// Frees the linked list, including its data using the supplied free_func
void free_list_data(list_t **list, void(free_func)(void *));
