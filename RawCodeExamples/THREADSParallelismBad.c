#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ARRAY_SIZE;

int main() {


    ARRAY_SIZE = 100000;

    //Initialize array
    int* array = (int*)malloc(ARRAY_SIZE * sizeof(int));
    long long sum = 0;

    // Initialize the array with values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += array[i];
    }
    
    printf("Sum: %lld\n", sum);

    free(array);

    return 0;
}
