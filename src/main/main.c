
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <errno.h>

#include <gtk/gtk.h>
#include <sqlite3.h>

#include "cmdline.h"
#include "trace.h"
// #include "errors.h"

void cmdline(int argc, char** argv, char** env) {

    init_cmdline("shop timer", "shop timer project", "0.1");
    add_cmdline('v', "verbosity", "verbosity", "From 0 to 10. Print more information", "0", NULL, CMD_NUM | CMD_ARGS);
    add_cmdline('p', "path", "path", "Add to the import path", "", NULL, CMD_STR | CMD_ARGS | CMD_LIST);
    add_cmdline('d', "dump", "dump", "Dump text as the parser is generated", "", NULL, CMD_STR | CMD_ARGS | CMD_LIST);
    add_cmdline('h', "help", NULL, "Print this helpful information", NULL, cmdline_help, CMD_NONE);
    add_cmdline('V', "version", NULL, "Show the program version", NULL, cmdline_vers, CMD_NONE);
    // add_cmdline(0, NULL, NULL, NULL, NULL, NULL, CMD_DIV);
    // add_cmdline(0, NULL, "files", "File name(s) to input", NULL, NULL, CMD_REQD | CMD_ANON);

    parse_cmdline(argc, argv, env);

    INIT_TRACE(NULL);
}

int main(int argc, char** argv, char** env) {

    cmdline(argc, argv, env);

    // FILE* fp = NULL;
    // const char* fname = raw_string(get_cmd_opt("files"));
    // if(fname != NULL) {
    //     fp = fopen(fname, "r");
    //     if(fp == NULL) {
    //         fprintf(stderr, "cannot open input file \"%s\": %s\n", fname, strerror(errno));
    //         cmdline_help();
    //     }
    //     fclose(fp);
    // }
    // else
    //     FATAL("internal error in %s: command line failed", __func__);

    gtk_init();

    sqlite3* db;
    sqlite3_open("test.db", &db);

    sqlite3_close(db);

    printf("Shop Timer\n");

    return 0;
}
