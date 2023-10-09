#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <papi.h>
#include <stdbool.h>
#include <string.h>

#define NUM_THREADS 6
#define CACHE_SIZE 64
char PAPI_Event[256];
long numLoops;

// Function to read the TSC (Time Stamp Counter)
unsigned long long rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

//Create a structure with padding, so that each structure fits in a cache line
typedef struct {
   long d;
   char buffer[CACHE_SIZE-sizeof(long)];
} Data;

Data *data;

void *thread_function( void *args )
{
   long i;
   long j = (long)args;

    // Measure execution time
    unsigned long long start_cycles, end_cycles;
    int eventset = PAPI_NULL;
    long_long values[1] = {(long_long) 0};
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
    for ( i = 0; i < numLoops; i++ )
    {
       data[j].d = i;
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
        printf("Usage: %s <number_loops>\n", argv[0]);
        printf("Usage: %s <PAPI_EVENT>\n", argv[1]);
        return 1;
    }

    numLoops = atoi(argv[1]);
    strcpy(PAPI_Event, argv[2]);

    //INITIALIZE STUFF
    long i;
    pthread_t ids[NUM_THREADS];
    double d;
    data = (Data*)malloc( sizeof(Data)*NUM_THREADS );

    //INITIALIZE PAPI
    if (strcmp(PAPI_Event, "none") != 0){
        PAPI_library_init(PAPI_VER_CURRENT);
        PAPI_thread_init( ( unsigned long ( * )( void ) ) ( pthread_self ) );
    }

    //FILL DATA STRUCTURE
    for ( i = 0; i < NUM_THREADS; i++ )
    {
        data[i].d = 0;
    }
    //CREATE THREADS
    for ( i = 0; i < NUM_THREADS; i++ )
    {
        pthread_create( ids+i, NULL, thread_function, (void*)i );
    }

    for ( i = 0; i < NUM_THREADS; i++ )
    {
        pthread_join( ids[i], NULL );
    }

    free(data);

    return 0;
}
