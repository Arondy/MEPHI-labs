#ifndef MENU
#define MENU
#include "splay_tree.h"

typedef int (*MenuAction)(Node**);

int menu(Node **root);

#endif