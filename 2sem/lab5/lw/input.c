#include <stdio.h>
#include <string.h>

int count_digits(const int num){
    char str[20];
    sprintf(str, "%d", num);
    return strlen(str);
}

int read_two_ui_lab5(unsigned int *x, unsigned int *y){
    int result = 0;
    do {
        result = scanf("%u %u", x, y);
        if (result == -1){
            printf("Program has been stopped\n");
            return -1;
        }
        if (result != 2){
            printf("You can't input such symbols.\n");
            scanf("%*[^\n]");
        }
    } while (result != 2);
    return 0;
}

int read_three_ui_lab5(unsigned int *x, unsigned int *y, unsigned int *type){
    int result = 0;
    do {
        result = scanf("%u %u %u", x, y, type);
        if (result == -1){
            printf("Program has been stopped\n");
            return -1;
        }
        if (result != 3){
            printf("You can't input such symbols.\n");
            scanf("%*[^\n]");
        } else if (*type > 2){
            printf("Type should be 0, 1 or 2!\n");
            result = 0;
        }
    } while (result != 3);
    return 0;
}

int read_four_ui_lab5(unsigned int *x1, unsigned int *y1, unsigned int *x2, unsigned int *y2){
    int result = 0;
    do {
        result = scanf("%u %u %u %u", x1, y1, x2, y2);
        if (result == -1){
            printf("Program has been stopped\n");
            return -1;
        }
        if (result != 4){
            printf("You can't input such symbols.\n");
            scanf("%*[^\n]");
        }
    } while (result != 4);
    return 0;
}

int read_input(int *input){
    int check = scanf("%d", input);

    while (check == 0){
        scanf("%*[^\n]");
        printf("You can't input such symbols.\n");
        check = scanf("%d", input);
    }

    if (check == -1){
        printf("Program has been stopped");
        return -1;
    }
    return 0;
}

int positive_input(int *input){
    int check = read_input(input);
    while (*input < 0 && check != -1){
        printf("You can't input negative values.\n");
        scanf("%*[^\n]");
        check = read_input(input);
    }
    if (check == -1){
        return -1;
    }
    return 0;
}

void clear_input_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_string_input(char *input_str){
    int check = scanf("%s", input_str);
    if (check == -1){
        return -1;
    }
    clear_input_buffer();
    return 0;
}