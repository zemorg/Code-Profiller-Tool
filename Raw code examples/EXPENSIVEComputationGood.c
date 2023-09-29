#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {

    int numLoops = 100000000;
    double x = 5.0;

    for (int i = 0; i < numLoops; i++) {
        double square1 = x * x;
    }

    return 0;
}
