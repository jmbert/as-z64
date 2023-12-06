#ifndef _ENCODINGS_H
#define _ENCODINGS_H

enum z64_reg_encodings
{
    Z64_REG_F,
    Z64_REG_A,
    Z64_REG_B,
    Z64_REG_C,
    Z64_REG_D,
    Z64_REG_E,
    Z64_REG_H,
    Z64_REG_L,
    Z64_REG_AF,
    Z64_REG_BC,
    Z64_REG_DE,
    Z64_REG_HL,
    Z64_REG_SP,
    Z64_REG_IX,
    Z64_REG_IY,
};

typedef unsigned short register_encoding_t;

enum z64_instr_opcodes
{
    Z64_OPCODE_STR,
    Z64_OPCODE_STRP,
    Z64_OPCODE_LD,
    Z64_OPCODE_LDL,
    Z64_OPCODE_LDP,
    Z64_OPCODE_LDLP,
    Z64_OPCODE_SWP,
    Z64_OPCODE_SWPP,
    Z64_OPCODE_ALT,
};

#endif