#include <pthread.h>
#include <stdlib.h>
#include "insertion_methods.h"

pthread_mutex_t global_lock;  // For Global Lock
pthread_mutex_t lock1, lock2; // For Two-Lock System

void* global_lock_insert(void* arg) {
    int* data = (int*)arg;  // Data to insert
    pthread_mutex_lock(&global_lock);  // Lock the entire list

    Node* new_node = (Node*)malloc(sizeof(Node)); // Traverse the list to find the correct insertion point
    new_node->value = *data;
    new_node->next = NULL;

    // Insert the node into the list in sorted order
    Node* current = head;
    Node* prev = NULL;

    while (current != NULL && current->value < *data) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {  // Insert at the head
        new_node->next = head;
        head = new_node;
    } else {  // Insert in between or at the end
        prev->next = new_node;
        new_node->next = current;
    }

    pthread_mutex_unlock(&global_lock);  // Unlock the list
    return NULL;
}

void* two_lock_insert(void* arg) {
    int* data = (int*)arg;  // Data to insert

    // Choose which lock to acquire
    if (*data <= NUM_NODES / 2) { // Values in the lower half
        pthread_mutex_lock(&lock1);
    } else { // Values in the upper half
        pthread_mutex_lock(&lock2);
    }
    // Traverse the list to find the correct insertion point
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->value = *data;
    new_node->next = NULL;

    // Insert the node into the list in sorted order
    Node* current = head;
    Node* prev = NULL;

    while (current != NULL && current->value < *data) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {  // Insert at the head
        new_node->next = head;
        head = new_node;
    } else {  // Insert in between or at the end
        prev->next = new_node;
        new_node->next = current;
    }

    // Release the lock
    if (*data <= NUM_NODES / 2) {
        pthread_mutex_unlock(&lock1);
    } else {
        pthread_mutex_unlock(&lock2);
    }
    return NULL;
}

void* node_lock_insert(void* arg) {
    int* data = (int*)arg;  // Data to insert
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->value = *data;
    new_node->next = NULL;

    // Special case: Inserting at the head
    pthread_mutex_lock(&head->lock);
    if (*data < head->value) {
        new_node->next = head;
        head = new_node;
        pthread_mutex_unlock(&head->lock);
        return NULL; // Exit
    }
    pthread_mutex_unlock(&head->lock);

    // Traverse the list to find the insertion point
    Node* current = head;
    pthread_mutex_lock(&current->lock);
    while (current->next != NULL && current->next->value < *data) {
        Node* temp = current->next;
        pthread_mutex_lock(&temp->lock);  // Lock the next node
        pthread_mutex_unlock(&current->lock);  // Unlock current node
        current = temp;  // Move to the next node
    }

    // Insert the new node
    new_node->next = current->next;
    current->next = new_node;

    pthread_mutex_unlock(&current->lock);  // Unlock the current node
    return NULL;
}