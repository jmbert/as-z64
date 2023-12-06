#ifndef _ENCODE_H
#define _ENCODE_H

#include <encodings.h>

typedef unsigned char z64_opcode_t;

enum z64_arguments
{
    Z64_ARGUMENT_REGISTER,
    Z64_ARGUMENT_REGISTERPAIR,
    Z64_ARGUMENT_MEMORY,
    Z64_ARGUMENT_LITERAL,
    Z64_ARGUMENT_LITERALPAIR,
};

enum z64_args_types
{
    Z64_ARGS_NONE,

    Z64_ARGS_REGISTER,
    Z64_ARGS_REGISTER_REGISTER,
    Z64_ARGS_REGISTER_MEMORY,
    Z64_ARGS_REGISTER_LITERAL,

    Z64_ARGS_REGISTERPAIR,
    Z64_ARGS_REGISTERPAIR_REGISTERPAIR,
    Z64_ARGS_REGISTERPAIR_MEMORY,
    Z64_ARGS_REGISTERPAIR_LITERALPAIR,

    Z64_ARGS_MEMORY,
    Z64_ARGS_LITERAL,
    Z64_ARGS_LITERALPAIR,
};

enum z64_arg_indices
{
    Z64_ARGS_STR =  Z64_ARGS_REGISTER_MEMORY,
    Z64_ARGS_STRP = Z64_ARGS_REGISTERPAIR_MEMORY,
    Z64_ARGS_LD =   Z64_ARGS_REGISTER_MEMORY,
    Z64_ARGS_LDL =  Z64_ARGS_REGISTER_LITERAL,
    Z64_ARGS_LDP =  Z64_ARGS_REGISTERPAIR_MEMORY,
    Z64_ARGS_LDLP = Z64_ARGS_REGISTERPAIR_LITERALPAIR,
    Z64_ARGS_SWP =  Z64_ARGS_REGISTER_REGISTER,
    Z64_ARGS_SWPP = Z64_ARGS_REGISTERPAIR_REGISTERPAIR,
    Z64_ARGS_ALT =  Z64_ARGS_NONE,
};

struct z64_operation_argument
{
    enum z64_arguments arg_type;
    union
    {   
        /* Z64_ARGUMENT_REGISTER || Z64_ARGUMENT_REGISTERPAIR */
        register_encoding_t reg; 

        /* Z64_ARGUMENT_MEMORY */
        unsigned long addr;

        /* Z64_ARGUMENT_LITERAL */
        unsigned short literal;

        /* Z64_ARGUMENT_LITERALPAIR */
        unsigned long literal_pair;

    }data;
};

struct z64_operation
{
    z64_opcode_t opcode;
    struct z64_operation_argument *args;
};

unsigned char *encode_operation(struct z64_operation op, unsigned int *oplen);

#endif