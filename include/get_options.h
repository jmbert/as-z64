#ifndef _GET_OPTIONS_H
#define _GET_OPTIONS_H

#include <stdio.h>

enum _options
{
    OPTION_VERBOSE = 1 << 0,
    OPTION_HELP = 1 << 1,
    OPTION_OUTPUT_PROVIDED = 1 << 2,
    OPTION_INPUT_PROVIDED = 1 << 3,
};

struct option_data
{
    FILE *output_file;
    FILE *input_file;
};

unsigned int get_options(struct option_data *opt_buffer, int argc, char **argv);

#endif