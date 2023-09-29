#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <papi.h>
#include <stdbool.h>

#define NUM_THREADS 6
#define CACHE_SIZE 64
long NUM_ITERATIONS;

char PAPI_EVENTS[4][256] = {
        "PAPI_L1_DCM",
        "PAPI_TOT_CYC",
        "PAPI_TOT_INS",
        "TSC"
    };

int PAPI_Event;
int retval;

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

    //PAPI INITIALIZATION
    int eventset=PAPI_NULL;
    long_long values[1] = {(long_long) 0};

    if (PAPI_Event != 3){
	retval = PAPI_register_thread(  );
	if ( retval != PAPI_OK ) {
	   printf("Thread register fail\n");
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
    }
    }

    start_cycles = rdtsc();
    //START ACTUAL CODE
    for ( i = 0; i < NUM_ITERATIONS; i++ )
    {
       data[j].d = i;
    }
    //END ACTUAL CODE

    //FINISH COUNTING
    end_cycles = rdtsc();

    if (PAPI_Event != 3){
        retval=PAPI_stop(eventset,values);
    }

    //PRINT RESULTS
    if (PAPI_Event == 3){
        printf("%llu\n", end_cycles - start_cycles);
    }else{
        printf("%lld\n", values[0]);
    }

   return NULL;
}


int main(int argc, char* argv[]) {
    //GET CONTROL VARIABLES
    if (argc != 3) {
        printf("Usage: %s <array_size>\n", argv[0]);
        printf("Usage: %s <PAPI_event_number>\n", argv[1]);
        return 1;
    }
    NUM_ITERATIONS = atoi(argv[1]);
    PAPI_Event = atoi(argv[2]);

    //INITIALIZE STUFF
    long i;
    pthread_t ids[NUM_THREADS];
    double d;

    retval=PAPI_library_init(PAPI_VER_CURRENT);
        if (retval!=PAPI_VER_CURRENT) {
                fprintf(stderr,"Error initializing PAPI! %s\n",
                        PAPI_strerror(retval));
                return 0;
        }

    data = (Data*)malloc( sizeof(Data)*NUM_THREADS );

    retval = PAPI_thread_init( ( unsigned long ( * )( void ) )
				   ( pthread_self ) );
	if ( retval != PAPI_OK ) {
		fprintf(stderr, "Error initializing threads");
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
