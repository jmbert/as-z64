#ifndef _AS_ERR_H
#define _AS_ERR_H

#include <stdio.h>
#include <stdlib.h>

enum _as_errors
{
    E_NOLINE,
    E_NOREG,
    E_NOOP,
    E_PARSEFAIL,
    E_ENCODEFAIL,
    E_ARGFAIL,
    E_UNCLEAR,
    E_BADTYPE,
};

#define as_err_nonfatal(_msg, ...) fprintf(stderr, _msg__VA_OPT__(,) __VA_ARGS__)

#define as_err_fatal(__exit_code, _msg, ...) \
    fprintf(stderr, _msg __VA_OPT__(,) __VA_ARGS__);\
    exit(-__exit_code)

#endif