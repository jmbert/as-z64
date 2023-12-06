#include <stdio.h>
#include <stdio.h>
#include <encode.h>
#include <get_options.h>
#include <parse.h>
#include <stdlib.h>
#include <as-err.h>
#include <string.h>

unsigned int verbose = 1;

void print_help(char *arg0)
{
    printf("Usage: %s [OPTIONS] file\n", arg0);
    printf("Options:\n");
    printf("\t-o\tSelect Output file (default a.out)\n");
    printf("\t-h\tDisplay this help message\n");
    printf("\t-v\tEnable verbose mode\n");
}

int main(int argc, char **argv)
{
    FILE *input = stdin;
    FILE *output = NULL;

    struct option_data opt = {};
    unsigned int optflags = get_options(&opt, argc, argv);
    if (optflags & OPTION_HELP)
    {
        print_help(argv[0]);
        exit(0);
    }
    if (optflags & OPTION_VERBOSE)
    {
        verbose = 1;
    }
    if (optflags & OPTION_OUTPUT_PROVIDED)
    {
        output = opt.output_file;
    } else
    {
        output = fopen("a.out", "w");
    }
    if (optflags & OPTION_INPUT_PROVIDED)
    {
        input = opt.input_file;
    }

    unsigned int len = 0;
    char *line = NULL;

    do
    {
        if (feof(input))
        {
            break;
        }
        unsigned int length = getline(&line, &len, input);
        struct z64_operation *op = parseline(line);
        if (op == NULL)
        {
            as_err_fatal(E_PARSEFAIL, "Unknown error while parsing \"%s\"\n", line);
        }
        unsigned int oplen = 0;
        unsigned char *encoding = encode_operation(*op, &oplen);
        if (encoding == NULL)
        {
            as_err_fatal(E_ENCODEFAIL, "Unknown error while encoding \"%s\"\n", line);
        }
        fwrite(encoding, sizeof(*encoding), oplen, output);
    } while (1);
    
    return 0;
}