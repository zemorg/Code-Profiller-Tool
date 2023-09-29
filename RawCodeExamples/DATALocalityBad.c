#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {

    int ARRAY_SIZE = 10000000;
    int cols = 16;

    //Init the array
    int **matrix = (int **)malloc(ARRAY_SIZE * sizeof(int *));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i + cols + j + 1; // Example values
        }
    }

    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            matrix[i][j] += 8;
        }
    }

    // Free allocated memory
    for (int i = 0; i < ARRAY_SIZE; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
