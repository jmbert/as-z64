#ifndef _PARSING_TYPES_H
#define _PARSING_TYPES_H

enum _tokens
{
    TOKEN_EOS,
    TOKEN_MNEM,
    TOKEN_REGISTER,
    TOKEN_LITERAL,
    TOKEN_MEMADDR,
};

struct token_info
{
    char *chrptr;
    unsigned int detail;
    enum _tokens type;
};

#endif