#include <stdio.h>

int inv(int x) {
    int r = 0, k = 0;
    while (x){
        r*=10,
        k=x%10,
        r+=k,
        x/=10;
    }
    return r;
}

int pal(int x) {
    int f = 0, p = 0;
    while (f == 0){
        p = inv(x);

        if (x==p){
            f = 1;
        }
        else{
            x += p;
        }
    }
    return x;
}

int main() {
    int x = 0;
    printf("Input a number:\n");
    scanf("%d", &x);
    printf("Palindrom: %d\n", pal(x));
    return 0;
}
