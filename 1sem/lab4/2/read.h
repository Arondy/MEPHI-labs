#ifndef READ
#define READ
#include <stddef.h>

size_t mystrlen(const char *s);
char *mystrdup(const char *original);
char *mystrtok(char *string, const char *sep);
char *mymemcpy (char *dest, const char *src, size_t len);
char *myreadline(const char *text);

#endif
