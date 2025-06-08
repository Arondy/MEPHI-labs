#include <stdlib.h>
#include "table.h"
#include "menu.h"

int main(){
    Table *table = malloc(sizeof(Table));
    menu(table);
    return 0;
}