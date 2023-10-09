#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <papi.h>

struct Data {
    int integer;
    float floating;
    char character;
};

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

    int size = atoi(argv[1]);
    char PAPI_Event[256];
    strcpy(PAPI_Event, argv[2]);

    //initalize stuff
    struct Data* data_array = (struct Data*)malloc(size * sizeof(struct Data));

    srand(time(NULL));

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
    // Generate and store random values in the struct array
    for (int i = 0; i < size; i++) {
        data_array[i].integer = rand() % 100;
        data_array[i].floating = (float)rand() / RAND_MAX * 100.0;
        data_array[i].character = 'A' + (rand() % 26);
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

    //printf("Array:\n");
    //for (int i = 0; i < size; i++) {
    //    printf("(%d, %.2f, %c) ", data_array[i].integer, data_array[i].floating, data_array[i].character);
    //}
    //printf("\n");

    // Don't forget to free the allocated memory when done
    free(data_array);

    return 0;
}
