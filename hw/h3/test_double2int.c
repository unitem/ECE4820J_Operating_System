#include <stdio.h>

int diff(double a, double b) {
    return a - b;
}

int main() {
    double a = 1.0;
    double b = 0.9;
    printf("%d\n", diff(a, b));
    return 0;
}