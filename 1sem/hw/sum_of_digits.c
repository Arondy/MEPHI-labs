#include <stdio.h>

int main() {
    int x = 0, res = 0;
    printf("Input a number:\n");
    scanf("%d", &x);
    while (x){
        res += x%10,
        x/=10;
    }
    printf("Sum of digits is: %d", res);
    return 0;
}
