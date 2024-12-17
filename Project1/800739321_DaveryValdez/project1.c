#include "project1.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2) // Check if the correct number of command-line arguments is provided
    {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }
    int num_processes = atoi(argv[1]);
    if (num_processes <= 0)
    {
        printf("Invalid number of processes: %d\n", num_processes);
        return 1;
    }
    struct timespec start, end;
    long long seconds, nano_seconds;

    /* ToDo1: Create and attach shared memory region 1 to store the matrix */
    int shm_id_matrix = shmget(IPC_PRIVATE, ROWS_COUNT * COLUMN_COUNT * sizeof(int), IPC_CREAT | 0666);
    if (shm_id_matrix < 0)
    {
        perror("shmget failed for matrix"); // Print error if shared memory creation fails
        exit(1);
    }
    int(*matrix)[COLUMN_COUNT] = shmat(shm_id_matrix, NULL, 0); // Attach the shared memory segment for the matrix to the process's address space
    if (matrix == (void *)-1)
    {
        perror("shmat failed for matrix"); // Print error if attaching shared memory fails
        exit(1);
    }

    /* ToDo2: Create and attach shared memory region 2 to store the partial sums */
    int shm_id_sums = shmget(IPC_PRIVATE, num_processes * sizeof(long), IPC_CREAT | 0666);
    if (shm_id_sums < 0)
    {
        perror("shmget failed for partial sums"); // Print if fails
        exit(1);
    }
    long *partial_sums = shmat(shm_id_sums, NULL, 0); // Attach the shared memory segment for partial sums to the process's address space
    if (partial_sums == (void *)-1)
    {
        perror("shmat failed for partial sums"); // Print if fails
        exit(1);
    }

    /* ToDo3: Initialize the shared memory region 1 to hold the matrix */
    for (int i = 0; i < ROWS_COUNT; i++) // Initialize the matrix with values such that matrix[i][j] = i + j
    {
        for (int j = 0; j < COLUMN_COUNT; j++)
        {
            matrix[i][j] = i + j;
        }
    }

    clock_gettime(CLOCK_REALTIME, &start);
    /* ToDo4: Create num_processes count of children processes */
    for (int p = 0; p < num_processes; p++) // Loop to create the specified number of child processes
    {
        pid_t pid = fork(); //new child process
        if (pid == 0)
        { // Child process
            // Calculate the starting and ending row indices for this child
            int start_row = p * (ROWS_COUNT / num_processes);
            int end_row = (p == num_processes - 1) ? ROWS_COUNT : (p + 1) * (ROWS_COUNT / num_processes); 

            // Convert integers to strings
            char start_row_str[20], end_row_str[20], shmid_matrix_str[20], shmid_sums_str[20], partial_sum_index_str[20];
            sprintf(start_row_str, "%d", start_row);
            sprintf(end_row_str, "%d", end_row);
            sprintf(shmid_matrix_str, "%d", shm_id_matrix);
            sprintf(shmid_sums_str, "%d", shm_id_sums);
            sprintf(partial_sum_index_str, "%d", p);

            // Use execlp to execute child_proc
            execlp("./child_proc", "./child_proc", start_row_str, end_row_str, shmid_matrix_str, shmid_sums_str, partial_sum_index_str, (char *)NULL);
            // If execlp returns, it must have failed
            perror("execlp failed");
            exit(1);
        }
        else if (pid < 0)
        {
            perror("fork failed"); //fork failed
            exit(1);
        }
    }

    /* ToDo5: Wait for all children processes to complete */
    for (int p = 0; p < num_processes; p++)
    {
        wait(NULL); // Wait for each child to finish
    }

    /* ToDo6: Sum all the partial sums stored in shared memory region 2 */
    long total_sum = 0;
    for (int p = 0; p < num_processes; p++)
    {
        total_sum += partial_sums[p];
    }
    printf("Total sum of matrix elements: %ld\n", total_sum);

    clock_gettime(CLOCK_REALTIME, &end);
    // Calculate execution time

    seconds = end.tv_sec - start.tv_sec;
    nano_seconds = end.tv_nsec - start.tv_nsec;
    long double execution_time = (long double)seconds + ((long double)nano_seconds / 1000000000);

    printf("Execution Time with %d processes: %Lf seconds\n", num_processes, execution_time); // Print execution time

    /* ToDo7: Detach and destroy both shared memory regions */
    shmdt(matrix);
    shmdt(partial_sums);
    shmctl(shm_id_matrix, IPC_RMID, NULL);
    shmctl(shm_id_sums, IPC_RMID, NULL);

    return 0; // Exit
}