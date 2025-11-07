
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cmdline.h"
#include "trace.h"

extern FILE* yyin; // would be defined in a FLEX scanner.

void cmdline(int argc, char** argv, char** env) {

    init_cmdline("template", "template project", "0.1");
    add_cmdline('v', "verbosity", "verbosity", "From 0 to 10. Print more information", "0", NULL, CMD_NUM | CMD_ARGS);
    add_cmdline('p', "path", "path", "Add to the import path", "", NULL, CMD_STR | CMD_ARGS | CMD_LIST);
    add_cmdline('d', "dump", "dump", "Dump text as the parser is generated", "", NULL, CMD_STR | CMD_ARGS | CMD_LIST);
    add_cmdline('h', "help", NULL, "Print this helpful information", NULL, cmdline_help, CMD_NONE);
    add_cmdline('V', "version", NULL, "Show the program version", NULL, cmdline_vers, CMD_NONE);
    add_cmdline(0, NULL, NULL, NULL, NULL, NULL, CMD_DIV);
    add_cmdline(0, NULL, "files", "File name(s) to input", NULL, NULL, CMD_REQD | CMD_ANON);

    parse_cmdline(argc, argv, env);

    INIT_TRACE(NULL);
}

int main(int argc, char** argv, char** env) {

    cmdline(argc, argv, env);

    const char* fname = raw_string(get_cmd_opt("files"));
    if(fname != NULL) {
        yyin = fopen(fname, "r");
        if(yyin == NULL) {
            fprintf(stderr, "cannot open input file \"%s\": %s\n", fname, strerror(errno));
            cmdline_help();
        }
    }
    else
        FATAL("internal error in %s: command line failed", __func__);

    printf("Hello Template: %s\n", argv[1]);

    return 0;
}
