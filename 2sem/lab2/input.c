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

int read_input3(char* token, int* id, int* arrival_time, int* required_time){
    int check = sscanf(token, "%d/%d/%d", id, arrival_time, required_time);

    while (*id <= 0 || *arrival_time < 0 || *required_time <= 0 || check != 3){
        if (check != 3){
            printf("%s\n", "Wrong data format!");
            return -1;
        }
        printf("You can't input negative values. Input ONLY ONE process again!\n");

        scanf("%s", token);
        check = sscanf(token, "%d/%d/%d", id, arrival_time, required_time);
    }
    return 0;
}