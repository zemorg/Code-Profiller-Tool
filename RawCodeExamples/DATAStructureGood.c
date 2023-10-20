#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Data {
    int integer;
    float floating;
    char character;
};

int main(int argc, char *argv[]) {

    int size = 1000000;

    //initalize array
    struct Data* data_array = (struct Data*)malloc(size * sizeof(struct Data));

    srand(time(NULL));

    // Generate and store random values in the struct array
    for (int i = 0; i < size; i++) {
        data_array[i].integer = rand() % 100;
        data_array[i].floating = (float)rand() / RAND_MAX * 100.0;
        data_array[i].character = 'A' + (rand() % 26);
    }

    printf("Array:\n");
    for (int i = 0; i < size; i++) {
        printf("(%d, %.2f, %c) ", data_array[i].integer, data_array[i].floating, data_array[i].character);
    }
    printf("\n");

    // Don't forget to free the allocated memory when done
    free(data_array);

    return 0;
}
