#ifndef PROJECT1_H
#define PROJECT1_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/time.h> 
#include <sys/ipc.h> 
#include <time.h>

#define ROWS_COUNT 32768
#define COLUMN_COUNT 32768

void print_matrix(int *matrix)
{
    for (int i = 0; i < ROWS_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            printf("%d\t", *(matrix + i * COLUMN_COUNT + j));
        }
        printf("\n");
    }
}

long sum_matrix(int start_row, int end_row, int *matrix)
{
    long sum = 0;
    /* ToDo1:
     * write the code for summing all elements from start_row to end_row
     */
    for (int i = start_row; i <= end_row; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            sum += *(matrix + i * COLUMN_COUNT + j);
        }
    }

    return sum;
}

#endif // PROJECT1_H
