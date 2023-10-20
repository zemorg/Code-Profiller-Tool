#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 6

int ARRAY_SIZE;

int* array;
long long* partial_sums;

// Function to calculate the partial sum for a range of elements
void* calculate_partial_sum(void* arg) {
    int thread_id = *(int*)arg;
    int chunk_size = ARRAY_SIZE / NUM_THREADS;
    int start = thread_id * chunk_size;
    int end = (thread_id == NUM_THREADS - 1) ? ARRAY_SIZE : (start + chunk_size);

    for (int i = start; i < end; i++) {
        partial_sums[thread_id] += array[i];
    }

    return NULL;
}

int main() {

    ARRAY_SIZE = 100000;

    //initialize array
    array = (int*)malloc(ARRAY_SIZE * sizeof(int));
    partial_sums = (long long*)malloc(NUM_THREADS * sizeof(long long));

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Initialize the array with values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    // Initialize partial_sums
    for (int i = 0; i < NUM_THREADS; i++) {
        partial_sums[i] = 0;
    }
    
    // Create and run threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, calculate_partial_sum, &thread_ids[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate the final sum from partial sums
    long long sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        sum += partial_sums[i];
    }

    printf("Sum: %lld\n", sum);

    free(array);
    free(partial_sums);

    return 0;
}
