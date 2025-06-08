#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "read.h"

#define STR "Your input: "

int bubble_sort(int *array, int len){
    for (int i = 0; i < len; i++) {
        for (int j = len - 1; j > i; j--) {
            if (array[j] < array[j - 1]) {
                int t = array[j - 1];
                array[j - 1] = array[j];
                array[j] = t;
            }
        }
    }
    return 0;
}

int remove_duplicates(int **array, int *len){
    for (int i = 0; i < (*len - 1); i++){
        if ((*array)[i] == (*array)[i + 1]){
            for (int k = i + 1; k < (*len - 1); k++){
                (*array)[k] = (*array)[k + 1];
            }
            i -= 1;
            *len -= 1;
        }
    }

    *array = realloc(*array, (*len) * sizeof(int));

    return 0;
}

char *process(const char *str){
    char *str_copy = mystrdup(str);
    int copy_len = mystrlen(str_copy);
    int max_ar_len = copy_len / 2 + 1;
    int used = 0;
    int words_num = 0;
    int word_len = 0;
    int *len_array = calloc(max_ar_len, sizeof(int));
    char **word_array = calloc(max_ar_len, sizeof(char *));
    char *result = calloc(copy_len + 1, sizeof(char));
    int out_len = 0;
    char *word = mystrtok(str_copy, " \t");
    while (word != NULL){
        len_array[used] = mystrlen(word);
        word_array[used] = word;
        used += 1;
        word = mystrtok(NULL, " \t");
    }
    words_num = used;
    len_array = realloc(len_array, used * sizeof(int));
    word_array = realloc(word_array, words_num * sizeof(char *));
    bubble_sort(len_array, used);
    if (words_num != 1){
        remove_duplicates(&len_array, &used);
    }

    for (int i = 0; i < used; i++){
        for (int j = 0; j < words_num; j++){
            word_len = mystrlen(word_array[j]);
            if (len_array[i] == word_len){
                mymemcpy(result + out_len, word_array[j], word_len * sizeof(char));
                out_len += word_len;
                result[out_len] = ' ';
                out_len++;
            }
        }
    }

    if (out_len > 0){
        out_len--;
    }

    result[out_len] = '\0';
    result = realloc(result, (out_len + 1) * sizeof(char));
    free(str_copy);
    free(len_array);
    free(word_array);

    return result;
}

int main(){
    char *input = myreadline(STR);
    while (input != NULL){
    	clock_t t0 = clock();
        printf("\"%s\"\n", input);
        char *result = process(input);
        printf("\"%s\"\n", result);
        free(input);
        free(result);
        clock_t t1 = clock();
        printf("%lf\n", (double)(t1 - t0) / CLOCKS_PER_SEC);
        input = myreadline(STR);
   	}
    return 0;
}
