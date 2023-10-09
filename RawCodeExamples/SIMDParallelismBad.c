#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {

    int ARRAY_SIZE = 1000000000;

    //INITIALIZE STUFF
    float *a = (float*)malloc(sizeof(float) * ARRAY_SIZE);
    float *b = (float*)malloc(sizeof(float) * ARRAY_SIZE);
    float *result = (float*)malloc(sizeof(float) * ARRAY_SIZE);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        result[i] = a[i] + b[i];
    }

    free(a);
    free(b);
    free(result);

    return 0;
}
