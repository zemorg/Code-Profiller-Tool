#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <papi.h>

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


    unsigned short int *vector = (unsigned short int *)malloc(sizeof(unsigned short int) * ARRAY_SIZE);

    // Fill the vector with random values between 0 and USHRT_MAX
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        vector[i] = rand() % (60000);
    }
    unsigned long sum = 0;

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
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += vector[i];
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

    free(vector);

    return 0;
}
