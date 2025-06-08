#include <stdio.h>
#include <math.h>
// Начиная с x = 5 результат крайне неточен

double exp1(int x, int n) {
    double s = 1, p = 1;
    int y = x * x;

    for (int i = 1; i <= n; i++) {
        p = -p * y / i;
        s += p;
    }
    return s;
}

double expw(int x, float e) {
    double s = 1, p = 1;
    int b = x * x, i = 1;

    while (fabs(p) > e) {
        p = -p * b / i;
        s += p;
        i += 1;
    }
    return s;
}

int main() {
    int x = 0;
    float y = 0;
    printf("Input two numbers: \n");
    scanf("%d%f", &x, &y);
    printf("%1.10lf -> %1.10lf", exp(-x * x), expw(x, y));
    return 0;
}
