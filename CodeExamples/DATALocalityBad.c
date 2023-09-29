#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#include <stdbool.h>

char PAPI_EVENTS[4][256] = {
        "PAPI_L1_DCM",
        "PAPI_TOT_CYC",
        "PAPI_TOT_INS",
        "TSC"
    };

// Function to read the TSC (Time Stamp Counter)
unsigned long long rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <array_size>\n", argv[0]);
        printf("Usage: %s <PAPI_event_number>\n", argv[1]);
        return 1;
    }

    // Convert the command-line argument to an integer
    int ARRAY_SIZE = atoi(argv[1]);
    int PAPI_Event = atoi(argv[2]);
    if (ARRAY_SIZE <= 0) {
        printf("Invalid array size. Please provide a positive integer.\n");
        return 1;
    }
    if (PAPI_Event < 0 || PAPI_Event > 3){
        fprintf (stderr, "Incorrect Event Number");
    }

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

    // Measure execution time
    unsigned long long start_cycles, end_cycles;

    //PAPI INITIALIZATION
    int retval, eventset = PAPI_NULL;
    long_long values[1] = {(long_long) 0};

    if (PAPI_Event != 3){
    retval=PAPI_library_init(PAPI_VER_CURRENT);
        if (retval!=PAPI_VER_CURRENT) {
                fprintf(stderr,"Error initializing PAPI! %s\n",
                        PAPI_strerror(retval));
                return 0;
        }

	retval=PAPI_create_eventset(&eventset);
	if (retval!=PAPI_OK) {
	   fprintf(stderr,"Error creating eventset! %s\n",
		PAPI_strerror(retval));
	}

	retval=PAPI_add_named_event(eventset, PAPI_EVENTS[PAPI_Event]);
	if (retval!=PAPI_OK) {
		fprintf(stderr,"Error adding PAPI_L1_DCM: %s\n",
		PAPI_strerror(retval));
	}

    //START COUNTING
    PAPI_reset(eventset);
    retval=PAPI_start(eventset);
    if (retval!=PAPI_OK) {
        fprintf(stderr,"Error starting COUNTING: %s\n",
        PAPI_strerror(retval));
    }}

    start_cycles = rdtsc();

    //START ACTUAL CODE
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            matrix[i][j] += 8;
        }
    }
    //END ACTUAL CODE

    end_cycles = rdtsc();

    //STOP COUNTING
    if (PAPI_Event != 3){
        retval=PAPI_stop(eventset,values);
    
        printf("%lld\n", values[0]);}
    else{
        printf("%llu\n", end_cycles - start_cycles);
    }

    // Free allocated memory
    for (int i = 0; i < ARRAY_SIZE; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
