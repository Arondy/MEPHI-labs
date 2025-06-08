#include <stdio.h>
#include <stdlib.h>

char *readline(char *buffer, int buffer_size) {
    printf("Your input:\n");
    int ix = 0, bytes_malloced = 0;

    if (!buffer)
    {
        bytes_malloced = 64;
        buffer = (char *)malloc(bytes_malloced);
        buffer_size = bytes_malloced;
    }

    for (;;++ix)
    {
        int ch;

        if (ix == buffer_size - 1)
        {
            if (!bytes_malloced)
                break;
            bytes_malloced += bytes_malloced;
            buffer = (char *)realloc(buffer, bytes_malloced);
            buffer_size = bytes_malloced;
        }

        ch = getchar();
        if (ch == EOF)
        {
            if (bytes_malloced)
                free(buffer);
            return NULL;
        }
        if (ch == '\n')
            break;
        buffer[ix] = ch;
    }

    /* 0 terminate buffer. */
    buffer[ix] = '\0';

    return buffer;
}