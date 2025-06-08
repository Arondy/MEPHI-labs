#include <stdio.h>
#include <math.h>

double lnf(double x, double a, int c){
    double p = x;
    double sum = x * cos(a);

    for (int n = 2; n <= c; n++){
        p = p * x / n;
        sum += p * cos(n * a);
    }
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
    double x = 0;
    double a = 0;
    int c = 0;
    printf("Input x, a, number of cycles: \n");
    scanf("%lf%lf%d", &x, &a, &c);
    x = check_input(x);
    printf("%1.10lf -> %1.10lf", log(1 - 2 * x * cos(a) + x * x), lnf(x, a, c));
    return 0;
}