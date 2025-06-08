#include <stdlib.h>
#include "table.h"
#include "menu.h"

int main(){
    Table table;
    table.mSize = 0;
    table.cSize = 0;
    table.maxSize = 10;
    table.ks = malloc(table.maxSize * sizeof(KeySpace));

    menu(table);

    return 0;
}