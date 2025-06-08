#include <stdio.h>

int ten_powered(int number){
    int t_i_p = 1;
    for(int i = number; i > 0; i--){
        t_i_p *= 10;
    }
    return t_i_p;
}

int number_of_digits(int number){
    int nod = 0, t_p = 1, num = number;

    while (num - 1 *t_p >= 0){
        t_p *= 10,
        nod += 1;
    }
    return nod;
}

int func(int number){
    int tenP = 0, res = 0, digit = 0, flag = 1, comparison_digit = 0, n_f = 0;
    int  max_nod = 0, curr_nod = 0;

    if (number < 0){
        number = -number,
        n_f = 1;
    }

    max_nod = number_of_digits(number),
    curr_nod = max_nod;

    while (curr_nod){
        tenP = ten_powered(curr_nod - 1),
        digit = number / tenP;

        while (digit >= 10){
            digit %= 10;
        }

        for (int i = curr_nod + 1; i <= max_nod; i++){
            comparison_digit = number / ten_powered(i-1);

            while (comparison_digit >= 10){
                comparison_digit %= 10;
            }

            if (digit == comparison_digit){
                flag = 0;
                break;
            }
        }

        if (flag){
            res += digit * tenP;
        }
        else{
            res /= 10;
        }

        curr_nod -= 1,
        flag = 1;
    }
    if (n_f){
        res = -res;
    }

    return res;
}

int main() {
    int number = 0;
    printf("Input your number:\n");
    scanf("%d", &number);
    printf("Your new number: %d\n", func(number));
    return 0;
}
