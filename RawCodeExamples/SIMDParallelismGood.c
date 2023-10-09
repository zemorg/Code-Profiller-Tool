#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <immintrin.h>

int main() {

    int ARRAY_SIZE = 1000000000;
    
    //INITIALIZE STUFF
    float *a = (float*)malloc(sizeof(float) * ARRAY_SIZE);
    float *b = (float*)malloc(sizeof(float) * ARRAY_SIZE);
    float *result = (float*)malloc(sizeof(float) * ARRAY_SIZE);
    int i;
    int vectorSize = 8; // For AVX (256 bits), assuming floats (4 bytes) -> 8 elements per vector
    // Calculate the number of vectors
    int vectorCount = ARRAY_SIZE / vectorSize;

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

    free(a);
    free(b);
    free(result);


    return 0;
}
