
#include <tokenise.h>
#include <encode.h>
#include <stdlib.h>
#include <string.h>
#include <parsing_types.h>
#include <opflags.h>
#include <as-err.h>

unsigned int determine_register_size(unsigned int reg)
{
    switch (reg)
    {
    case Z64_REG_A:
    case Z64_REG_F:
    case Z64_REG_B:
    case Z64_REG_C:
    case Z64_REG_D:
    case Z64_REG_E:
        return Z64_ARGUMENT_REGISTER;
        break;
    case Z64_REG_AF:
    case Z64_REG_BC:
    case Z64_REG_DE:
    case Z64_REG_HL:
    case Z64_REG_SP:
    case Z64_REG_IX:
    case Z64_REG_IY:
        return Z64_ARGUMENT_REGISTERPAIR;
        break;
    default:
        return -1;
        break;
    }
}

unsigned int determine_opcode(struct z64_operation_argument *args, unsigned int arglen, unsigned int mnemon)
{
    switch (mnemon)
    {
    case MNEMON_ALT:
        if (arglen != 0)
        {
            as_err_fatal(E_UNCLEAR, "Invalid argument number %d for mnemonic #%d\n", arglen, mnemon);
        }
        return Z64_OPCODE_ALT;
        break;

    case MNEMON_SWP:
        if (arglen != 2)
        {
            as_err_fatal(E_UNCLEAR, "Invalid argument number %d for mnemonic #%d\n", arglen, mnemon);
        }
        if (args[0].arg_type != args[1].arg_type)
        {
            as_err_fatal(E_UNCLEAR, "Invalid combination of arguments #%d and #%d for mnemonic #%d\n", args[0].arg_type, args[1].arg_type, mnemon);
        }
        unsigned int swapsize = args[0].arg_type;
        if (swapsize == Z64_ARGUMENT_REGISTER)
        {
            return Z64_OPCODE_SWP;
        } else if (swapsize = Z64_ARGUMENT_REGISTERPAIR)
        {
            return Z64_OPCODE_SWPP;
        } else 
        {
            as_err_fatal(E_UNCLEAR, "Unknown register size #%d for mnemonic #%d\n", swapsize, mnemon);
        }
    case MNEMON_STR:
        if (arglen != 2)
        {
            as_err_fatal(E_UNCLEAR, "Invalid argument number %d for mnemonic #%d\n", arglen, mnemon);
        }
        if ((args[0].arg_type != Z64_ARGUMENT_REGISTER && args[0].arg_type != Z64_ARGUMENT_REGISTERPAIR) ||
            args[1].arg_type != Z64_ARGUMENT_MEMORY)
        {
            as_err_fatal(E_UNCLEAR, "Invalid combination of arguments #%d and #%d for mnemonic #%d\n", args[0].arg_type, args[1].arg_type, mnemon);
        }
        unsigned int strsize = args[0].arg_type;
        if (strsize == Z64_ARGUMENT_REGISTER)
        {
            return Z64_OPCODE_STR;
        } else if (strsize = Z64_ARGUMENT_REGISTERPAIR)
        {
            return Z64_OPCODE_STRP;
        } else 
        {
            return -1;
        }
    case MNEMON_LD:
        if (arglen != 2)
        {
            as_err_fatal(E_UNCLEAR, "Invalid argument number %d for mnemonic #%d\n", arglen, mnemon);
        }
        if ((args[0].arg_type != Z64_ARGUMENT_REGISTER && args[0].arg_type != Z64_ARGUMENT_REGISTERPAIR) ||
            (args[1].arg_type != Z64_ARGUMENT_MEMORY && args[1].arg_type != Z64_ARGUMENT_LITERAL && args[1].arg_type != Z64_ARGUMENT_LITERALPAIR))
        {
            as_err_fatal(E_UNCLEAR, "Invalid combination of arguments #%d and #%d for mnemonic #%d\n", args[0].arg_type, args[1].arg_type, mnemon);
        }

        if (args[1].arg_type != Z64_ARGUMENT_MEMORY && 
            ((args[0].arg_type == Z64_ARGUMENT_REGISTER && args[1].arg_type != Z64_ARGUMENT_LITERAL) ||
            (args[0].arg_type == Z64_ARGUMENT_REGISTERPAIR && args[1].arg_type != Z64_ARGUMENT_LITERALPAIR)))
        {
            as_err_fatal(E_UNCLEAR, "Invalid combination of arguments #%d and #%d for mnemonic #%d\n", args[0].arg_type, args[1].arg_type, mnemon);
        }

        int ldlit = !(args[1].arg_type == Z64_ARGUMENT_MEMORY);
        unsigned int size = args[0].arg_type;
    
        if (!ldlit & size == Z64_ARGUMENT_REGISTER)
        {
            return Z64_OPCODE_LD;
        } else if (ldlit & size == Z64_ARGUMENT_REGISTER)
        {
            return Z64_OPCODE_LDL;
        } else if (!ldlit & size == Z64_ARGUMENT_REGISTERPAIR)
        {
            return Z64_OPCODE_LDP;
        } else if (ldlit & size == Z64_ARGUMENT_REGISTERPAIR)
        {
            return Z64_OPCODE_LDLP;
        } else {
            return -1;
        }
    default:
        return -1;
        break;
    }
}

/*
 * This function parses the arguments, given a operation mnemonic.
 * It should also use this to determine the final opcode
 */

struct z64_operation_argument *parse_args(unsigned int op_mnemon, unsigned int *opcode_buf, char **line_p)
{
    if (line_p == NULL)
    {
        return NULL;
    }
    struct z64_operation_argument *args = (typeof(args))malloc(sizeof(*args)*strlen(*line_p));
    struct z64_operation_argument current_arg = {};
    struct token_info current_info = {};

    unsigned int expected_size = 0;
    unsigned int *unknown_size = NULL;

    int i = 0;
    do
    {
        int result = get_token(line_p, &current_info);
        if (result == -1 || result == TOKEN_MNEM)
        {
            as_err_fatal(E_PARSEFAIL, "Unknown error while parsing \"%s\"\n", *line_p);
        }
        if (result == TOKEN_EOS)
        {
            break;
        }

        switch (current_info.type)
        {
        case TOKEN_REGISTER:
            unsigned int size = determine_register_size(current_info.detail);
            if (size == -1)
            {
                as_err_fatal(E_ARGFAIL, "Failed to determine size of register \"%s\"\n", current_info.chrptr);
            }
            current_arg.arg_type = size;
            unsigned int literalsize = 0;
            if (size = Z64_ARGUMENT_REGISTER)
            {
                literalsize = Z64_ARGUMENT_LITERAL;
            } else if (size = Z64_ARGUMENT_REGISTERPAIR)
            {
                literalsize = Z64_ARGUMENT_LITERALPAIR;
            }
            expected_size = literalsize;
            if (unknown_size != NULL)
            {
                *unknown_size = literalsize;
            }
            
            current_arg.data.reg = current_info.detail;
            break;
        case TOKEN_LITERAL:
            if (expected_size == 0)
            {
                unknown_size = &(args[i].arg_type);
            }
            current_arg.arg_type = expected_size;
            if (current_arg.arg_type == Z64_ARGUMENT_LITERAL)
            {
                current_arg.data.literal= atoi(current_info.chrptr);
            } else
            {
                current_arg.data.literal_pair = atoi(current_info.chrptr);
            }
            break;
        case TOKEN_MEMADDR:
            current_arg.arg_type = Z64_ARGUMENT_MEMORY;
            current_arg.data.addr = atoi(current_info.chrptr);
            break;
        default:
            as_err_fatal(E_UNCLEAR, "Could not determine type of argument \"%s\"\n", current_info.chrptr);
            break;
        }

        args[i] = current_arg;

        i++;
    } while (1);


    int result = determine_opcode(args, i, op_mnemon);
    if (result == -1)
    {
        as_err_fatal(E_UNCLEAR, "Could not determine form of mnemnomic #%d\n", op_mnemon);
    }
    *opcode_buf = result;

    return args;
}