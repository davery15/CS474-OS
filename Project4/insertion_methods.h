#ifndef INSERTION_METHODS_H
#define INSERTION_METHODS_H

#include "linked_list.h"

// Mutexes for insertion methods
extern pthread_mutex_t global_lock;
extern pthread_mutex_t lock1;
extern pthread_mutex_t lock2;

// Insert values using different locking strategies
void* global_lock_insert(void* arg);
void* two_lock_insert(void* arg);
void* node_lock_insert(void* arg);

#endif // INSERTION_METHODS_H
