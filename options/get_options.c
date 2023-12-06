#include <get_options.h>
#include <getopt.h>
#include <as-err.h>

unsigned int get_options(struct option_data *opt_buffer, int argc, char **argv)
{
    unsigned int opflags = 0;

    int opt;

    while ((opt = getopt(argc, argv, "o:hv")) != -1) {
        switch (opt) {
        case 'o': 
            opflags |= OPTION_OUTPUT_PROVIDED; 
            opt_buffer->output_file = fopen(optarg, "w");
            break;
        case 'h': 
            opflags |= OPTION_HELP; 
            break;
        case 'v':
            opflags |= OPTION_VERBOSE; 
            break;
        }
    }

    if (optind < argc)
    {
        opflags |= OPTION_INPUT_PROVIDED;
        opt_buffer->input_file = fopen(argv[optind], "r");
    }

    return opflags;
}