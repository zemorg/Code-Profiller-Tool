#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <papi.h>

#define NUM_THREADS 6
char PAPI_Event[256];
int ARRAY_SIZE;

// Function to read the TSC (Time Stamp Counter)
unsigned long long rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

int* array;
long long* partial_sums;

// Function to calculate the partial sum for a range of elements
void* calculate_partial_sum(void* arg) {
    int thread_id = *(int*)arg;
    int chunk_size = ARRAY_SIZE / NUM_THREADS;
    int start = thread_id * chunk_size;
    int end = (thread_id == NUM_THREADS - 1) ? ARRAY_SIZE : (start + chunk_size);

    // Measure execution time
    unsigned long long start_cycles, end_cycles;
    long_long values[1] = {(long_long) 0};
    int eventset = PAPI_NULL;
    
    //PAPI INITIALIZATION
    if (strcmp(PAPI_Event, "none") != 0){
        PAPI_create_eventset(&eventset);
        PAPI_add_named_event(eventset, PAPI_Event);

        //START COUNTING
        PAPI_reset(eventset);
        PAPI_start(eventset);
    }

    start_cycles = rdtsc();

    //START ACTUAL CODE
    for (int i = start; i < end; i++) {
        partial_sums[thread_id] += array[i];
    }
    //END ACTUAL CODE

    //STOP COUNTING
    end_cycles = rdtsc();
    if (strcmp(PAPI_Event, "none") != 0){
        PAPI_stop(eventset,values);
    }
    
    //RETURN RESULTS
    printf("%llu\n", end_cycles - start_cycles);
    if (strcmp(PAPI_Event, "none") != 0){
        printf("%lld\n", values[0]);}

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <Array_Size>\n", argv[0]);
        printf("Usage: %s <PAPI_EVENT>\n", argv[1]);
        return 1;
    }

    ARRAY_SIZE = atoi(argv[1]);
    strcpy(PAPI_Event, argv[2]);

    //initialize stuff
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

    // Measure execution time
    //PAPI INITIALIZATION
    if (strcmp(PAPI_Event, "none") != 0){
        PAPI_library_init(PAPI_VER_CURRENT);
        PAPI_thread_init( ( unsigned long ( * )( void ) ) ( pthread_self ) );
    }

    //START ACTUAL CODE
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

    //END ACTUAL CODE

    //STOP COUNTING
    //end_cycles = rdtsc();
    //if (strcmp(PAPI_Event, "none") != 0){
    //    PAPI_stop(eventset,values);
   // }
    
    //RETURN RESULTS
    //printf("%llu\n", end_cycles - start_cycles);
    //if (strcmp(PAPI_Event, "none") != 0){
     //   printf("%lld\n", values[0]);}

    //printf("Sum: %lld\n", sum);

    free(array);
    free(partial_sums);

    return 0;
}
