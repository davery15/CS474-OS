#include "project1.h"

int main(int argc, char *argv[]) {

    if (argc != 6) {
        printf("Usage: %s <starting_row_number> <number_of_rows_to_sum> shmid_matrix shmid_partial_sum partial_sum_index\n", argv[0]);
        return 1;
    }

    int start_row_id = atoi(argv[1]);
    int end_row_id = atoi(argv[2]);
    int shmid_matrix = atoi(argv[3]);
    int shmid_partial_sum = atoi(argv[4]);
    int partial_sum_index = atoi(argv[5]);

    if ((start_row_id < 0) ||(start_row_id>=ROWS_COUNT)) {
        printf("Invalid starting row id: %d\n", start_row_id);
        return 1;
    }


   // ToDo1: Attach the shared memory region for the matrix
    int (*matrix)[COLUMN_COUNT] = shmat(shmid_matrix, NULL, 0);
    if (matrix == (void *) -1) {
        perror("shmat failed for matrix");
        return 1;
    }

    // ToDo2: Attach the shared memory region for the partial sums
    long *partial_sums = shmat(shmid_partial_sum, NULL, 0);
    if (partial_sums == (void *) -1) {
        perror("shmat failed for partial sums");
        shmdt(matrix);
        return 1;
    }

    // ToDo3: Calculate the sum of elements from start_row_id to end_row_id using sum_matrix
    long sum = sum_matrix(start_row_id, end_row_id -1, (int *)matrix);

    // ToDo4: Store the sum in the appropriate index of the partial sums
    partial_sums[partial_sum_index] = sum;

    // Detach both shared memory regions before leaving the main
    shmdt(matrix);
    shmdt(partial_sums);


    exit(0);
}
