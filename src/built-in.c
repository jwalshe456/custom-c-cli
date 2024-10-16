/* 

********************************************************************
Student Name: Jed Walshe
Student Number: 21338606
I acknowledge DCU's Academic Integrity Policy. 
All code and text submitted is my own, unless stated otherwise, in such case it is properly referenced.

********************************************************************

*/

// implementation and handling of commands called by users of the shell

/* HEADER FILES */
#include "built-in.h"
#include "utility.h"

/* GLOBAL VARIABLES */
extern char dir_buf[MAX_BUFFER]; // original located in env.c
bool paused; // variable to check if use of shell should be paused
extern char *prompt; // global var for command prompt

/* FUNCTION LISTS */

void (*pfunc1_list[])(char**, int) = {
    &dir,
    &help,
    &echo
};

void (*pfunc2_list[])(void) = {
    &quit,
    &clr,
    &env,
    &switch_pause
};

char *func_list[] = {
    "cd",
    "dir",
    "help",
    "echo",
    "quit",
    "clr",
    "environ",
    "pause"
};

/* 
function: echo
arguments: char** "args": list of arguments,
    int "len": no of arguments
description: prints strings that follow it
returns: void
*/

void echo(char **args, int len){

    // check for remnants of i/o redirection
    if(args[len-2] == NULL){
        len -= 2;
    }

    // print each argument followed by a space
    int i = 1;
    while(i < len){
        fprintf(stdout, "%s", args[i]);
        if (i+1 < len){
            fputs(" ", stdout);
        }
        ++i;
    }
    fprintf(stdout, "\n");
}

/* 
function: quit
arguments: void
description: exits shell
returns: void
*/

void quit(void){
    printf("Exitting program...\n");
    exit(0);
}

/* 
function: clr
arguments: void
description: clears screen
returns: void
*/

void clr(void){
    // \033c is the octal for the
    // escape character (man 7 ascii)
    printf("%s", "\033c");
}

/* 
function: env
arguments: void
description: prints each entry of environment
    variables
returns: void
*/

void env(void){

    extern char **environ; // buffer to hold environment variable string
    int i = 0;
    while(environ[i]){
        fprintf(stdout, "%s\n", environ[i]);
        ++i;
    }
}

/* 
function: switch_pause
arguments: void
description: changes the state of the global paused variable 
returns: void
*/

void switch_pause(void){
    paused = !paused;
    if (paused){
        fprintf(stdout, "Press Enter to unpause shell\n");
    }

}

/* 
function: cd
arguments: char* "directory": a named directory
description: changes directory and sets
    related environment variables (PWD, OLDPWD)
returns: int, denotes success
*/

void cd(char **args) {
    char *dir = args[1];
    char *cwd = getcwd(dir_buf, MAX_BUFFER);
    // ensure directory exists
    if (dir != NULL){

        if (check_file_type(dir) == IS_DIR){
            

            // change directory and set OLDPWD
            chdir(dir); 
            setenv("OLDPWD", cwd, 1);

            // set PWD
            cwd = getcwd(dir_buf, MAX_BUFFER); 
            setenv("PWD", cwd, 1);
        }

        else {
        	fprintf(stdout, "\"%s\": not a directory\n", dir);
        }
    }

    else{
        fprintf(stdout, "%s\n", cwd);
    }
    //update command prompt
    prompt = get_prompt(); 
}

/* 
function: help
arguments: char** "args": a list of arguments, 
    int "len": no of arguments
description: displays the manual pages for
    built-in commands using more, while
    also adding each pages full path
returns: void
*/

void help(char **args, int len){
    
    // set default value
    if (len == 1){
        args[1] = "readme";
    }

    // change file name to include path
    char *man_dir = getenv("MAN_DIR");
    int buf_len = strlen(man_dir) + strlen(args[1]) + 3;
    char buf[buf_len];

    snprintf(buf, buf_len, "%s/%s", man_dir, args[1]);

    char *cmds[] = {"more", "-d", buf, NULL};
    fork_exec(cmds);
}

/* 
function: dir
arguments: char** "args": a list of arguments
    int "len": no of arguments
description: prints the file size and name of each entry
    in a given directory
returns: void
*/

void dir(char **args, int len){

    // ensure room for -alv and NULL pointer
    // at end of arg list
    char *cmds[len+3];
    cmds[0] = "ls";
    cmds[1] = "-alv";

    // add orignal arguments to 
    // new array
    if(1 < len){
        int i = 1;
        while(i < len+1){
            cmds[i+1] = args[i];
            ++i;
        }
        cmds[i] = NULL;
    }
    else{
        cmds[2] = NULL;
    }

    fork_exec(cmds);
}

/* 
function: run_command
arguments: char** "args": a list of arguments
description: checks for validity of commands, 
    unaliasing some and altering the arguments of others
returns: int, denotes success
*/

int run_command(char **args, int len){

    extern bool dont_wait;
    int rv = -1;
    // save stdin and stdout in the event
    // of i/o redirection
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

    // check i/o redirection
    parse(args, &len);

    // if command isn't built-in
    if (!built_in(args, len)){
        char* file = *args;
        char bin_file[260];

        strcpy(bin_file, "/bin/");
        
        // ensure that command is of valid length
        if (255 <= strlen(file)){
            fprintf(stderr, "%s : name too long, length must not exceed 255\n", file);
            rv = 1;
        }
        strcat(bin_file, file);

        // run executable/binary files
        if (check_file_type(file) == IS_EXEC || check_file_type(bin_file) == IS_EXEC){ 
            rv = 0;
            fork_exec(args);
        }

        else{
            print_not_valid(args, len);   
        }
    }
    
    // return stdin and stdout to their previous states
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);

    dont_wait = false;

    //if not valid will return non-zero
    return rv;
}

/* 
function: print_not_valid
arguments: char** "args": a list of arguments
description: prints invalid line to stdout 
returns: void
*/

void print_not_valid(char **args, int len){
    fputs("\"", stderr);
    int i = 0;
    while (i < len) {
        fprintf(stderr,"%s",args[i]);
        if (i+1 < len){
            fputs(" ", stderr);
        }
        ++i;
    }
    fputs("\": not a valid command\n", stderr);
    fprintf(stderr, "type \"help\" for more information\n");
}

bool built_in(char **args, int len){

    // function pointers for built-ins
    void (*pfunc1)(char**, int);
    void (*pfunc2)(void);
    void (*pfunc3)(char**);
    
    int i = 0;
    // loop breaks when command matches built-in or
    // if i exceeds no of built-ins
    while (i < 8 && strcmp(args[0],func_list[i])){++i;}
    if (i < 8){

        // function with one arg
        if (i == 0){
            pfunc3 = &cd;
            pfunc3(args);
        }
        
        // function with two args
        else if (i < 4){
            pfunc1 = pfunc1_list[i-1];
            pfunc1(args, len);
        }
        
        // function with no args
        else if (4 <= i){
            pfunc2 = pfunc2_list[i-4];
            pfunc2();
        }
        
        return true;
    }

    return false;
}