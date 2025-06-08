#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//char *process(char *str){
//    char *new_str = calloc(1000, sizeof(char));
//    char *str_copy = strdup(str);
//    int len = 0;
//    if (str){
//        char *word = strtok(str_copy, " \t");
//
//        while (word){
//            int count = 0;
//            while (word[count] != '\000'){
//                new_str[len] = word[count];
//                if (word[count] >= 'A' && word[count] <= 'Z'){
//                    len += 1;
//                    new_str[len] = word[count];
//                }
//                len += 1;
//                count += 1;
//            }
//            word = strtok(NULL, " \t");
//            if (word){
//                new_str[len] = ' ';
//                len += 1;
//            }
//        }
//    }
//    return new_str;
//}

char *process(char *str){
    char *new_str = calloc(1000, sizeof(char));
    char *str_copy = strdup(str);
    int len = 0;
    int count = 0;
    int wordlen = 0;

    char *word = strtok(str_copy, " ");
    while (word){
        wordlen = strlen(word);
        memcpy(&new_str[len], word, wordlen);
        len += wordlen;
        if (count == 1){
            new_str[len] = ' ';
            len += 1;
            count = -1;
            memcpy(&new_str[len], word, wordlen);
            len += wordlen;
        }
        word = strtok(NULL, " ");
        if (word){
            new_str[len] = ' ';
            len += 1;
        }
        count += 1;
    }

    return new_str;
}

int main(){
    char *str = "   LEJWDJNWJNK          DNKWDN r wds th   ";
    char *new_str = process(str);
    int count = 0;
    while (new_str[count] != '\0'){
        printf("%c", new_str[count]);
        count++;
    }
    return 0;
}