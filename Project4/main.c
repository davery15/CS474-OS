#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "linked_list.h"
#include "insertion_methods.h"

#define NUM_THREADS 16

int insert_values[NUM_THREADS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// Function to calculate elapsed time
double calculate_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    pthread_mutex_init(&global_lock, NULL);
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    printf("Initializing list...\n");
    initialize_list();
    printf("Original List:\n");
    display_list();

    pthread_t threads[NUM_THREADS];
    struct timespec start, end;
    double elapsed_time;

    // Using Global Lock
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, global_lock_insert, &insert_values[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
         pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed_time = calculate_time(start, end);
    printf("After Global Lock Insertion:\n");
    display_list();
    printf("Global Lock Insertion Time: %f seconds\n", elapsed_time);

    // Reset and test Two-Lock system
    cleanup_list();
    initialize_list();
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, two_lock_insert, &insert_values[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed_time = calculate_time(start, end);
    printf("After Two-Lock Insertion:\n");
    display_list();
    printf("Two-Lock Insertion Time: %f seconds\n", elapsed_time);

    // Reset and test Node-Level Lock system
    cleanup_list();
    initialize_list();
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, node_lock_insert, &insert_values[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed_time = calculate_time(start, end);
    printf("After Node-Level Lock Insertion:\n");
    display_list();
    printf("Node-Level Lock Insertion Time: %f seconds\n", elapsed_time);

    // Cleanup
    pthread_mutex_destroy(&global_lock);
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    cleanup_list();

    return 0;
}
