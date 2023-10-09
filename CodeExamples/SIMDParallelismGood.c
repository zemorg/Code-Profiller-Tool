#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#include <stdbool.h>
#include <string.h>
#include <immintrin.h>

// Function to read the TSC (Time Stamp Counter)
unsigned long long rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number_loops>\n", argv[0]);
        printf("Usage: %s <PAPI_EVENT>\n", argv[1]);
        return 1;
    }

    int ARRAY_SIZE = atoi(argv[1]);
    char PAPI_Event[256];
    strcpy(PAPI_Event, argv[2]);
    
    //INITIALIZE STUFF
    float *a = (float*)malloc(sizeof(float) * ARRAY_SIZE);
    float *b = (float*)malloc(sizeof(float) * ARRAY_SIZE);
    float *result = (float*)malloc(sizeof(float) * ARRAY_SIZE);
    int i;
    int vectorSize = 8; // For AVX (256 bits), assuming floats (4 bytes) -> 8 elements per vector
    // Calculate the number of vectors
    int vectorCount = ARRAY_SIZE / vectorSize;

    // Measure execution time
    unsigned long long start_cycles, end_cycles;
    int eventset = PAPI_NULL;
    long_long values[1] = {(long_long) 0};
    //PAPI INITIALIZATION
    if (strcmp(PAPI_Event, "none") != 0){
    
        PAPI_library_init(PAPI_VER_CURRENT);
        PAPI_create_eventset(&eventset);
        PAPI_add_named_event(eventset, PAPI_Event);

        //START COUNTING
        PAPI_reset(eventset);
        PAPI_start(eventset);
    }

    start_cycles = rdtsc();

    //START ACTUAL CODE
    for (i = 0; i < vectorCount; i++) {
        // Load vectors from arrays
        __m256 vectorA = _mm256_loadu_ps(&a[i * vectorSize]);
        __m256 vectorB = _mm256_loadu_ps(&b[i * vectorSize]);

        // Perform SIMD addition
        __m256 vectorResult = _mm256_add_ps(vectorA, vectorB);

        // Store the result back to the array
        _mm256_storeu_ps(&result[i * vectorSize], vectorResult);
    }

    // Calculate remaining elements (if size is not a multiple of vectorSize)
    for (i = vectorCount * vectorSize; i < ARRAY_SIZE; i++) {
        result[i] = a[i] + b[i];
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

    free(a);
    free(b);
    free(result);


    return 0;
}
