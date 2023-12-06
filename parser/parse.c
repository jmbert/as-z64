#include <encode.h>
#include <tokenise.h>
#include <parsing_types.h>
#include <stdlib.h>
#include <parse_args.h>
#include <as-err.h>

struct z64_operation *parseline(char *line)
{
    struct z64_operation *op = (typeof(op))malloc(sizeof(*op));

    struct token_info info = {};

    int result = get_token(&line, &info);
    if (result == -1 || result != TOKEN_MNEM)
    {
        as_err_fatal(E_PARSEFAIL, "Unknown error while parsing \"%s\"\n", line);
    }

    
    op->args = parse_args(info.detail, &op->opcode, &line);
    if (op->args == NULL)
    {
        as_err_fatal(E_PARSEFAIL, "Unknown error while parsing \"%s\"\n", line);
    }

    return op;
}