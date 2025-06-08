#include <stdio.h>
#include <stdlib.h>

size_t mystrlen(const char *s){
    int res = 0;
    while (*s) {
        s++;
        res++;
    }
    return res;
}

char *mystrdup(const char *original){
	char *duplicate = NULL;
	char *offset = NULL;
	int size = mystrlen(original);
	
	duplicate = calloc(size + 1, sizeof(char));
	
	if (duplicate == NULL){
		return NULL;
	}
	
	offset = duplicate;	// чтоб сохранить исходный указатель в duplicate
	
	while (*original){
		*offset = *original;
		offset++;
		original++;
	}
	*offset = '\0';

	return duplicate;
}

int is_sep(char c, const char *sep){
	while(*sep != '\0'){
    	if(c == *sep){
    		return 1;
    	}
   		sep++;
  	}
  	return 0;
}

char *mystrtok(char *string, const char *sep){
  	static char *p;
    char *word;

  	if (!string){
        string = p;
  	}

  	if (!string){
    	return NULL;
 	}

  	while (1){
    	if (is_sep(*string, sep)){ // Если нет разделителей в начале, то break
      		string++;
      		continue;
    	}
    	if (*string == '\0'){
      		return NULL;
    	} 
    	break;
  	}

  	word = string;
  	
  	while (1){
    	if (*string == '\0'){ // Конец строки
      		p = string;
     		return word;
    	}
    	if (is_sep(*string, sep)){
      		*string = '\0';
      		p = string + 1;
      		return word;
    	}
    	string++;
  	}
}

void mymemcpy(char *dest, const char *src, size_t len){
    while (len){
        *dest = *src;
        dest++;
        src++;
        len--;
    }
}

char *myreadline(const char *text){
	printf("%s", text);
	char timed[81] = {0};
	int len = 0;
	int check;
	char *buffer = NULL;

	do {
		check = scanf("%80[^\n]", timed);

		if (check == -1){
	        free(buffer);
	        printf("Program has been stopped\n");
	        _Exit (EXIT_SUCCESS);
	    }
		else if (check > 0){
			int chunk_len = mystrlen(timed);
			int str_len = len + chunk_len;
			buffer = realloc(buffer, str_len + 1);
			mymemcpy(buffer + len, timed, chunk_len * sizeof(char));
			len = str_len;
		}
	    else {
            scanf("%*c");
        }
	}
	while (check > 0);
	if (len > 0){
		buffer[len] = '\0';
	}
	else {
        buffer = calloc(1, sizeof(char));
    }
    return buffer;
}
