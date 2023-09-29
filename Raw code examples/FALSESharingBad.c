#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 6
long NUM_ITERATIONS;

long *data;

void *thread_function( void *args )
{
    long i;
    long j = (long)args;

    for ( i = 0; i < NUM_ITERATIONS; i++ )
    {
        data[j] = i;
    }

    return NULL;
}


int main() {
    
    NUM_ITERATIONS = 100000000;

    //INITIALIZE STUFF
    long i;
    pthread_t ids[NUM_THREADS];
    double d;

    data = (long*)malloc( sizeof(long)*NUM_THREADS );

    //FILL DATA ARRAY
    for ( i = 0; i < NUM_THREADS; i++ )
    {
        data[i] = 0;
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
