#include <encode.h>

#include <stdlib.h>
#include <string.h>
#include <as-err.h>

unsigned char *encode_operation(struct z64_operation op, unsigned int *oplen)
{
    enum z64_arg_indices argtype;
    


    switch (op.opcode)
    {
    case Z64_OPCODE_LD: 
        argtype = Z64_ARGS_LD; 
        break;
    case Z64_OPCODE_LDL: 
        argtype = Z64_ARGS_LDL; 
        break;
    case Z64_OPCODE_LDP: 
        argtype = Z64_ARGS_LDP; 
        break;
    case Z64_OPCODE_LDLP: 
        argtype = Z64_ARGS_LDLP; 
        break;
    case Z64_OPCODE_STR: 
        argtype = Z64_ARGS_STR; 
        break;
    case Z64_OPCODE_STRP: 
        argtype = Z64_ARGS_STRP; 
        break;
    case Z64_OPCODE_SWP: 
        argtype = Z64_ARGS_SWP; 
        break;
    case Z64_OPCODE_SWPP: 
        argtype = Z64_ARGS_SWPP; 
        break;
    case Z64_OPCODE_ALT: 
        argtype = Z64_ARGS_ALT; 
        break;
    default: return NULL; break;
    }

    unsigned int arglen;
    switch (argtype)
    {
    case Z64_ARGS_NONE: 
        arglen = 0; 
        break;

    case Z64_ARGS_REGISTER:
    case Z64_ARGS_REGISTERPAIR:
    case Z64_ARGS_MEMORY:
    case Z64_ARGS_LITERAL:
    case Z64_ARGS_LITERALPAIR:
        arglen = 1;
        break;

    case Z64_ARGS_REGISTER_REGISTER:
    case Z64_ARGS_REGISTER_MEMORY:
    case Z64_ARGS_REGISTER_LITERAL:
    case Z64_ARGS_REGISTERPAIR_REGISTERPAIR:
    case Z64_ARGS_REGISTERPAIR_MEMORY:
    case Z64_ARGS_REGISTERPAIR_LITERALPAIR:
        arglen = 2;
        break;

    default:
        return NULL;
        break;
    }  

    unsigned char *operation_base = (unsigned char *)malloc(sizeof(z64_opcode_t) + sizeof(struct z64_operation_argument) * arglen);
    unsigned char *operation = operation_base;
    memcpy(operation, &op.opcode, sizeof(op.opcode));
    operation += sizeof(op.opcode)/sizeof(unsigned char);

    for (unsigned int i = 0; i < arglen; i++)
    {
        struct z64_operation_argument arg = op.args[i];
        
        switch (arg.arg_type)
        {
        case Z64_ARGUMENT_REGISTERPAIR:
        case Z64_ARGUMENT_REGISTER:
            memcpy(operation, &arg.data.reg, sizeof(arg.data.reg));
            operation += sizeof(arg.data.reg)/sizeof(unsigned char);
            break;
        case Z64_ARGUMENT_LITERAL:
            memcpy(operation, &arg.data.literal, sizeof(arg.data.literal));
            operation += sizeof(arg.data.literal)/sizeof(unsigned char);
            break;
        case  Z64_ARGUMENT_MEMORY:
            memcpy(operation, &arg.data.addr, sizeof(arg.data.addr));
            operation += sizeof(arg.data.addr)/sizeof(unsigned char);
            break;
        case Z64_ARGUMENT_LITERALPAIR:
            memcpy(operation, &arg.data.literal_pair, sizeof(arg.data.literal_pair));
            operation += sizeof(arg.data.literal_pair)/sizeof(unsigned char);
            break;
        default:
            as_err_fatal(E_BADTYPE, "Unkown type #%d\n", arg.arg_type);
            break;
        }
    }

    *oplen = operation-operation_base;

    return operation_base;
}