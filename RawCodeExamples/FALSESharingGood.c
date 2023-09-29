#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 6
#define CACHE_SIZE 64
long NUM_ITERATIONS;

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

    for ( i = 0; i < NUM_ITERATIONS; i++ )
    {
       data[j].d = i;
    }

   return NULL;
}


int main() {
    
    NUM_ITERATIONS = 100000000;

    //INITIALIZE STUFF
    long i;
    pthread_t ids[NUM_THREADS];
    double d;

    data = (Data*)malloc( sizeof(Data)*NUM_THREADS );

    //FILL DATA STRUCTURE
    for ( i = 0; i < NUM_THREADS; i++ )
    {
        data[i].d = 0;
    }

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
