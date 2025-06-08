#include <stdlib.h>
#include "hashtable.h"
#include "menu.h"

int main(){
    KeySpace *ks = malloc(MAXSIZE * sizeof(KeySpace));
    for (int i = 0; i < MAXSIZE; i++){
        ks[i].busy = 0;
        ks[i].key = NULL;
    }

    menu(ks);

    return 0;
}