#include <stdio.h>
#include <string.h>

int count_digits(const int num){
    char str[20];
    sprintf(str, "%d", num);
    return strlen(str);
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
    while (*input <= 0 && check != -1){
        printf("You can't input negative values.\n");
        scanf("%*[^\n]");
        check = read_input(input);
    }
    if (check == -1){
        return -1;
    }
    return 0;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_string_input(char *input_str) {
    int check = scanf("%s", input_str);
    if (check == -1){
        return -1;
    }
    clear_input_buffer();
    return 0;
}
