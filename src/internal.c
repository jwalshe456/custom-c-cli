/* 

********************************************************************
Student Name: Jed Walshe
Student Number: 21338606
I acknowledge DCU's Academic Integrity Policy. 
All code and text submitted is my own, unless stated otherwise, in such case it is properly referenced.

********************************************************************

*/

// implementation and handling of commands called by users of the shell

#include "internal.h"
#include "env.h"

// global variables for this file
extern char **environ; // buffer to hold environment variable string
extern char dir_buf[MAX_BUFFER]; // original located in env.c
bool paused; // variable to check if use of shell should be paused
char *prompt; // global var for command prompt

/* 
function: switch_pause
arguments: void
description: changes the state of the global paused variable 
returns: void
*/

void switch_pause(void){

    paused = !paused;
}

/* 
function: cd
arguments: char* "directory": a named directory
description: changes directory and sets
    related environment variables (PWD, OLDPWD)
returns: int, denotes success
*/

int cd(char *directory) {

    char *cwd = getcwd(dir_buf, MAX_BUFFER);
    // ensure directory exists
    if (check_file_type(directory) == IS_DIR){
        

        // change directory and set OLDPWD
        chdir(directory); 
        setenv("OLDPWD", cwd, 1);

        // set PWD
        cwd = getcwd(dir_buf, MAX_BUFFER); 
        setenv("PWD", cwd, 1);
    }

    else if (directory != NULL){

    	fprintf(stdout, "\"%s\": not a directory\n", directory);
    }

    else{
        fprintf(stdout, "%s\n", cwd);
    }
    //update command prompt
    prompt = get_prompt(); 
    return 0;
}

/* 
function: print_man
arguments: char** "args": a list of arguments
description: displays the manual pages for
    internal commands using more, while
    also adding each pages full path
returns: char**, the updated list of arguments
*/

char **print_man(char **args){
    
    // default file to be opened
    if(args[1] == NULL){
        args[1] = "readme";
    }

    // detection of i/o redirection for this command
    int i = 0;
    while(args[i] && args[i][0] != '>'){++i;}
    
    // shifting arguments for 2 cases of output redirection:
    //   - default man file to selected output file,
    //   - or selected man file(s) to selected output file
    if (i == 1){
        args[i+3] = args[i+1];
        args[i+2] = args[i];
        args[i] = "readme";
        ++i;
    }

    else {
        args[i+2] = args[i+1];
        args[i+1] = args[i];
        args[i] = args[i - 1];
    }
    
    // get path to directory containing manuals
    // path is set on initialisation of shell
    char *man_dir = getenv("MAN_DIR");
    char *buf;
    
    
    // change file names to full path for file,
    // allowing for use anywhere on system
    while(1 < i) {

        buf = malloc(strlen(man_dir) + strlen("/") + strlen(args[i-1]));
        if (!buf){
            syserr("memory for buffer couldn't be allocated");
        }
        sprintf(buf, "%s%s%s", man_dir, "/", args[i-1]);
        args[i] = buf;
        buf = NULL;
        --i;
    }
    buf = NULL;
    free(buf);

    // convert command from "help" to "more -d"
    args[0] = "more";
    args[1] = "-d";
    return args;
}

/* 
function: format_dir
arguments: char** "args": a list of arguments
description: shifts arguments right by 1, adds "-alv"
    option to dir
returns: char**, the updated list of arguments
*/

char **format_dir(char **args){
    int i = 0;
    while(args[i]){
        ++i;
    }

    while(strcmp(args[i-1], "dir")){
        args[i] = args[i-1];
        --i;
    }
    args[1] = "-alv";

    return args;
}

/* 
function: run_command
arguments: char** "args": a list of arguments
description: checks for validity of commands, 
    unaliasing some and altering the arguments of others
returns: int, denotes success
*/

int run_command(char **args){

    int rv = -1;
    
    // independent commands: quit, pause, cd
    if (!strcmp(args[0],"quit"))
        exit(0);

    if (!strcmp(args[0],"pause")){
        switch_pause();
        rv = 0;
    }

    if (!strcmp(args[0],"cd")){
        rv = cd(args[1]);
    }

    // commands with reformatted arguments: help, dir
    if (!strcmp(args[0],"help")){
        print_man(args);
    }

    if (!strcmp(args[0],"dir")){
        format_dir(args);
    }
    
    // aliased commands: clr, environ
    if (!strcmp(args[0],"clr")) {
        args[0] = "clear";
    }

    if (!strcmp(args[0],"environ")){
        args[0] = "env";
    }

    if (rv != 0){
        char* file = *args;
        char bin_file[260];

        strcpy(bin_file, "/bin/");
        
        // ensure that command is of valid length
        if (255 <= strlen(file)){
            fprintf(stdout, "%s : name too long, length must not exceed 255\n", file);
            rv = 1;
        }
        strcat(bin_file, file);

        // run executable/binary files
        if (check_file_type(file) == IS_EXEC || check_file_type(bin_file) == IS_EXEC){ 
            rv = parse_run(args);
            
        }
    }

    //if not valid will return non-zero
    return rv;
}

/* 
function: print_not_valid
arguments: char** "args": a list of arguments
description: prints invalid line to stdout 
returns: void
*/

void print_not_valid(char **args){
    fputs("\"", stderr);
    char **arg = args;
    while (*arg) {
        fprintf(stderr,"%s",*arg++);
        if (*arg){
            fputs(" ", stderr);
        }
    }
    fputs("\": not a valid command\n", stderr);
    fprintf(stderr, "type \"help\" for more information\n");
}
