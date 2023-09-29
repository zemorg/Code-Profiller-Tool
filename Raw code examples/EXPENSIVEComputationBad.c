#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int main() {

    double x = 5.0;
    int numLoops = 100000000;

    for (int i = 0; i < numLoops; i++) {
        double square2 = pow(x, 2);
    }

    return 0;
}
