#include <tokenise.h>

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <encodings.h>
#include <opflags.h>
#include <tokenise_helpers.h>
#include <as-err.h>

#define TEST_ID(__str, __result) \
if (!strcmp(ident, __str))\
{\
    return __result;\
}\

int determine_register(char *ident)
{
    TEST_ID("A", Z64_REG_A);
    TEST_ID("F", Z64_REG_F);
    TEST_ID("B", Z64_REG_B);
    TEST_ID("C", Z64_REG_C);
    TEST_ID("D", Z64_REG_D);
    TEST_ID("E", Z64_REG_E);
    TEST_ID("H", Z64_REG_H);
    TEST_ID("L", Z64_REG_L);
    TEST_ID("AF", Z64_REG_AF);
    TEST_ID("BC", Z64_REG_BC);
    TEST_ID("DD", Z64_REG_DE);
    TEST_ID("HL", Z64_REG_HL);
    TEST_ID("IX", Z64_REG_IX);
    TEST_ID("IY", Z64_REG_IY);
    TEST_ID("SP", Z64_REG_SP);
    return -1;
}

int determine_operation(char *ident)
{
    TEST_ID("STR", MNEMON_STR);
    TEST_ID("LD", MNEMON_LD);
    TEST_ID("SWP", MNEMON_SWP);
    TEST_ID("ALT", MNEMON_ALT);
    return -1;
}

#undef TEST_REG_ID

#define idlen() _idlen(line)

static inline int _idlen(char *_line)
{
    int l = 0;
    while (!isspace(*_line) && *_line != '\0') 
    {
        l++;
        _line++;
    } ;
    return l;
}

#define line ((*linep)+offset)

#define copy(__test, __buffer) \
    for (typeof(__buffer) __buffer_2 = __buffer;__test(*line) && *line != '\0';__buffer_2++, offset++) \
    {  \
        if (__buffer_2 != NULL)\
        { \
            (*(char*)__buffer_2) = *line;\
        }\
    } \


#define skip(__test) copy(__test, NULL)

#define __test_compare(__char, __sent, __compare) (__char __compare __sent)
#define _test_nendident !isspace
#define _test_nendline(__char) __test_compare(__char, '\n', !=)

int get_token(char **linep, struct token_info *tokinfo)
{
    int token = -1;
    unsigned int offset = 0;
    if (linep == NULL)
    {
        as_err_fatal(E_NOLINE, "Tokenisation: Invalid Line Pointer\n");
    }

    skip(isspace);
    if (!*line)
    {
        token = TOKEN_EOS;
        goto ret;
    }

    char c = *line;

    /* Determine token type */
    /* Either a register, literal, memory address or mnemnonic */
    if (c == ';')
    {
        token = TOKEN_EOS;
        skip(_test_nendline);
        goto ret;
    } else if (c == '~')
    {
        token = TOKEN_REGISTER;
        offset++;
        tokinfo->chrptr = (char*)malloc(idlen());
        copy(_test_nendident, tokinfo->chrptr);
        int reg = determine_register(strupr(tokinfo->chrptr));
        if (reg == -1) 
        {
            as_err_fatal(E_NOREG, "Tokenisation: No such Register \"%s\"\n", tokinfo->chrptr);
        }
        tokinfo->detail = reg;
        goto ret;
    } else if (c == '#')
    {
        token = TOKEN_LITERAL;
        offset++;
        tokinfo->chrptr = (char*)malloc(idlen());
        copy(_test_nendident, tokinfo->chrptr);
        tokinfo->detail = atoi(tokinfo->chrptr);
        goto ret;

    } else if (c == '@')
    {
        offset++;
        token = TOKEN_MEMADDR;
        tokinfo->chrptr = (char*)malloc(idlen());
        copy(_test_nendident, tokinfo->chrptr);
        tokinfo->detail = atoi(tokinfo->chrptr);
        goto ret;
    } else
    {
        token = TOKEN_MNEM;
        tokinfo->chrptr = (char*)malloc(idlen());
        copy(_test_nendident, tokinfo->chrptr);
        int mnemon = determine_operation(strupr(tokinfo->chrptr));
        if (mnemon == -1) 
        {
            as_err_fatal(E_NOOP, "Tokenisation: No such Mnemonic \"%s\"\n", strupr(tokinfo->chrptr));
        }
        tokinfo->detail = mnemon;
        goto ret;
    }

ret:
    (*linep)+=offset;
    return tokinfo->type = token;

};