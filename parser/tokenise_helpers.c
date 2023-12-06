
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char *strupr(char *s)
{
    char *s2 = (char*)malloc(strlen(s));
    strcpy(s2, s);
    char *p = s2;
    while (*p) 
    {
        *p = toupper(*p);
        ++p;
    }
    return s2;
}   