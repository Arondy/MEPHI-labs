#include <stdio.h>
#include <math.h>

double lnw(double x, double a, double e, int *count){
    int n = 1;
    double p0 = 0;
    double p = x;
    double sum = x * cos(a);

    while (fabs(p * cos(n * a) - p0 * cos((n - 1) * a)) > e){
        n += 1;
        p0 = p;
        p = p * x / n;
        sum += p * cos(n * a);
    }
    *count = n;
    return -2 * sum;
}

double check_input(double x){
    while (fabs(x) > 1){
        printf("x value should belong to [-1; 1] range!\n");
        printf("Input x: \n");
        scanf("%lf", &x);
    }
    return x;
}

int main(){
    int count = 0;
    double x = 0;
    double a = 0;
    double e = 0;
    printf("Input x, a, delta: \n");
    scanf("%lf%lf%lf", &x, &a, &e);
    x = check_input(x);
    printf("%1.10lf -> %1.10lf\nSteps needed: %d", log(1 - 2 * x * cos(a) + x * x), lnw(x, a, e, &count), count);
    return 0;
}