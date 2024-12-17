#include "scheduling_algorithms.h"

// Compare-And-Swap function using GCC built-in atomic operation
bool compare_and_swap(int *location, int expected, int new_value)
{
    int current_value = __sync_val_compare_and_swap(location, expected, new_value);
    // If the value at 'location' was equal to 'expected', the swap occurred, so return true
    return current_value == expected;
    // ToDo1: Implement the compare-and-swap logic using GCC's built-in atomic operation.
    // Hint: We use an atomic operation to prevent race conditions when multiple threads try to update shared data.
    // Use __sync_val_compare_and_swap(location, expected, new_value).
    // Compare the value at 'location' with 'expected'. If they are equal, update 'location' to 'new_value'.
    // Return true if the swap happened, and false otherwise.
}

void calculate_times(ThreadData *threads)
{
    // ToDo2: Calculate waiting times, turnaround times, and completion times for each thread.
    // - Waiting time: Time the thread spends waiting before its execution starts.
    // - Turnaround time: Time from arrival to completion (Turnaround time = Waiting time + Burst time).
    // - Completion time: The actual time when the thread completes (Completion time = Turnaround time + Arrival time).
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Turnaround time = Completion time - Arrival time
        threads[i].turnaround_time = threads[i].completion_time - threads[i].arrival_time;

        // Waiting time = Turnaround time - Burst time
        threads[i].waiting_time = threads[i].turnaround_time - threads[i].burst_time;
    }
}

void print_results(ThreadData *threads)
{
    /* ToDo3: Implement a function to print the results of the scheduling algorithms.
     * Display the waiting time, turnaround time, completion time, and sum for each thread.
     */
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %d:\n", threads[i].tid);
        printf("  Waiting Time: %d\n", threads[i].waiting_time);
        printf("  Turnaround Time: %d\n", threads[i].turnaround_time);
        printf("  Completion Time: %d\n", threads[i].completion_time);
        printf("  Sum: %d\n", threads[i].sum);
        printf("\n");
    }
}

void calculate_sum(ThreadData *thread)
{

    /* ToDo4: Implement the logic to calculate the sum of values in the thread's array.
     * Iterate over the 'array' of the thread and compute the sum.
     *Store the result in the thread's 'sum' field.
     */

    int sum = 0;
    for (int i = 0; i < thread->burst_time; i++) {
        sum += thread->array[i];
    }
    thread->sum = sum;
}

void *fcfs_thread(void *arg)
{
    /* ToDo5: Implement the logic to simulate the execution of the thread in FCFS.
     * Use 'compare_and_swap' to mark the thread as running. Calculate its sum, simulate execution using 'usleep', and reset its scheduler state.
     */
    ThreadData *thread = (ThreadData *)arg;
    // Use Compare-And-Swap to mark the thread as running
    if (compare_and_swap(&thread->scheduler_array[thread->tid], 0, 1)) {
        // Calculate the sum of the thread's array
        calculate_sum(thread);

        // Simulate execution time (burst time) in microseconds
        usleep(thread->burst_time * 1000);

        // Mark as completed and set the completion time
        thread->completion_time = thread->waiting_time + thread->burst_time + thread->arrival_time;

        // Release the scheduler slot
        thread->scheduler_array[thread->tid] = 0;
    }

    return NULL;
}

void fcfs(ThreadData *threads)
{
    /* ToDo6: Implement the First-Come, First-Served (FCFS) scheduling algorithm.
     * - Assign waiting times based on the order of arrival.
     * - Create and join threads.
     * - Calculate and print the scheduling results.
     */

   pthread_t tid[NUM_THREADS];
    int current_time = 0;  // Keeps track of the current time (when each thread finishes)
    
    for (int i = 0; i < NUM_THREADS; i++) {
        // Ensure threads are executed in FCFS order (one after another)
        if (current_time < threads[i].arrival_time) {
            current_time = threads[i].arrival_time;  // Fast forward to the arrival time of the next thread
        }

        // Waiting Time = Current Time - Arrival Time
        threads[i].waiting_time = current_time - threads[i].arrival_time;

        // Update the thread's completion time and turnaround time based on the burst time
        threads[i].turnaround_time = threads[i].waiting_time + threads[i].burst_time;
        threads[i].completion_time = current_time + threads[i].burst_time;

        // Create the thread to simulate execution
        pthread_create(&tid[i], NULL, fcfs_thread, &threads[i]);

        // Wait for the thread to complete
        pthread_join(tid[i], NULL);

        // Move current_time forward by the burst time of this thread
        current_time += threads[i].burst_time;
    }

    calculate_times(threads);
    print_results(threads);
}

void *sjf_thread(void *arg)
{
    /* ToDo7: Implement the logic to simulate execution of the thread in SJF.
     * Calculate the thread's sum, simulate execution using 'usleep', and print its status.
     */
    ThreadData *thread = (ThreadData *)arg;

    // Calculate the sum for the thread
    calculate_sum(thread);

    // Simulate execution based on burst time
    usleep(thread->burst_time * 1000);

    // Mark completion time
    thread->completion_time = thread->waiting_time + thread->burst_time + thread->arrival_time;

    return NULL;
}

void sjf(ThreadData *threads)
{
    /* ToDo8: Implement the Shortest Job First (SJF) scheduling algorithm.
     * - Sort the threads based on their burst times.
     * - Assign waiting times, create threads, and join them.
     * - Calculate and print the scheduling results.
     */

    for (int i = 0; i < NUM_THREADS - 1; i++) {
        for (int j = i + 1; j < NUM_THREADS; j++) {
            if (threads[i].burst_time > threads[j].burst_time) {
                ThreadData temp = threads[i];
                threads[i] = threads[j];
                threads[j] = temp;
            }
        }
    }

    pthread_t tid[NUM_THREADS];

    // Create and start threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL, sjf_thread, &threads[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    calculate_times(threads);
    print_results(threads);
}

void srjf(ThreadData *threads)
{
    /* ToDo9: Implement the Shortest Remaining Job First (SRJF) scheduling algorithm.
     * - Use a time variable and continuously check for the thread with the shortest remaining burst time.
     * - Implement preemption by switching to the thread with the shortest remaining time.
     * - After all threads have completed, calculate and print the scheduling results
     */
    int current_time = 0;    // Keeps track of the current simulation time
    int completed = 0;       // Keeps track of how many threads have completed
    int min_time;            // The shortest remaining burst time
    int shortest;            // Index of the thread with the shortest remaining time
    bool found;              // To check if any thread is ready to be processed

    while (completed < NUM_THREADS) {
        shortest = -1;
        min_time = 100000;  // A large number to initialize min_time
        found = false;

        // Find the thread with the shortest remaining burst time that's ready to run
        for (int i = 0; i < NUM_THREADS; i++) {
            if (threads[i].arrival_time <= current_time && threads[i].remaining_time > 0) {
                found = true;
                if (threads[i].remaining_time < min_time) {
                    min_time = threads[i].remaining_time;
                    shortest = i;
                }
            }
        }

        if (!found) {
            // If no thread is found (i.e., no thread has arrived yet), increment time
            current_time++;
            continue;
        }
        // Print running thread and remaining time
        printf("Thread %d is now running (Remaining Time: %d)\n", threads[shortest].tid, threads[shortest].remaining_time);

        // Simulate execution of the thread with the shortest remaining burst time
        threads[shortest].remaining_time--;
        current_time++;

        // If this is the first time the thread is running, calculate the sum of the array
        if (threads[shortest].remaining_time == threads[shortest].burst_time - 1) {
            calculate_sum(&threads[shortest]);
        }

        // If the thread has finished its execution
        if (threads[shortest].remaining_time == 0) {
            completed++;
            threads[shortest].completion_time = current_time;
            printf("Thread %d has finished execution.\n", threads[shortest].tid);

            // Calculate turnaround time and waiting time for this thread
            threads[shortest].turnaround_time = threads[shortest].completion_time - threads[shortest].arrival_time;
            threads[shortest].waiting_time = threads[shortest].turnaround_time - threads[shortest].burst_time;
        }
    }
    calculate_times(threads);
    print_results(threads);
}
