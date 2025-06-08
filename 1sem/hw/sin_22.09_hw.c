#include <stdio.h>
#include <math.h>
// Начиная с x = 35 результат неточен на 0.01, погрешность растёт с повышением числа

double sinus(double x, double e){
	double s = x, p = x, b = x * x;
	int i = 3;

    while (fabs(p) > e){
        p = -p * b / i / (i-1);
        s += p;
        i += 2;
    }
	return s;
}

int main(){
	double x = 0, e = 0;
	printf("Input two numbers: \n");
	scanf("%lf%lf", &x, &e);
	printf("%1.10lf -> %1.10lf", sin(x), sinus(x, e));
    return 0;
}
