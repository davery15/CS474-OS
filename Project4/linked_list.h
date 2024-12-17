#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <pthread.h>

#define NUM_NODES 100


typedef struct Node {
    int value;
    struct Node* next;
    pthread_mutex_t lock;  // For Node-Level Lock
} Node;

// Global pointer to the head of the linked list
extern Node* head;

// Linked list functions
void initialize_list();
void display_list();
void cleanup_list();

#endif // LINKED_LIST_H
