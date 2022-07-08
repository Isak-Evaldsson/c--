#ifndef LIST_H
#define LIST_H

// Simple generic linked list implementation
typedef struct list list_t;

struct list {
    void** value;
    list_t* next;
};

list_t* create_list(void* value, list_t* next);

#endif /* LIST_H */
