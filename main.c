/* 

strtokeg - skeleton shell using strtok to parse command line
usage:
strtokeg
reads in a line of keyboard input at a time, parsing it into
tokens that are separated by white spaces (set by #define SEPARATORS).
can use redirected input if the first token is a recognised internal command, 
then that command is executed. otherwise the tokens are printed on the display.

internal commands:
clear - clears the screen
quit - exits from the program
********************************************************************
version: 1.0
date:    December 2003
author:  Ian G Graham
School of Information Technology
Griffith University, Gold Coast
ian.graham@griffith.edu.au
copyright (c) Ian G Graham, 2003. All rights reserved.
This code can be used for teaching purposes, but no warranty,
explicit or implicit, is provided.
*******************************************************************/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators


pid_t waitpid(pid_t pid, int *status_ptr, int options);
char *getcwd(char *buf, size_t size);
char *getwd(char *buf);
char *get_current_dir_name(void);
void fork_exec(char **args);
void syserr(char * msg);
void set_shell_env(void);

extern char **environ;

int main (int argc, char ** argv)
{

    char buf[MAX_BUFFER];                      // line buffer
    char *args[MAX_ARGS];                     // pointers to arg strings
    char **arg;                               // working pointer thru args
    char *prompt = "~~>";               // shell prompt
    bool valid;
    set_shell_env();
    /* keep reading input until "quit" command or eof of redirected input */

    while (!feof(stdin)) { 
        /* get command line from input */
        fputs (prompt, stdout); // write prompt

        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);   // tokenise input

            while ((*arg++ = strtok(NULL,SEPARATORS)));

            // last entry will be NULL

            if (args[0]) {                     // if there's anything there
                /* check for internal/external command */
                valid = false;
                if (!strcmp(args[0],"quit"))   // "quit" command
                    exit(0);                     // break out of 'while' loop
                
                if (!strcmp(args[0],"dir")){   // "dir" alias
                    args[0] = "ls";
                    valid = true;
                }
                
                if (!strcmp(args[0],"clear")) { // "clear" command
                    valid = true;
                    
                }

                if (!strcmp(args[0],"environ")){   // "environ" alias

                    args[0] = "env";
                    valid = true;
                }

                if (!strcmp(args[0],"cd")){   // "cd" command
                    if (args[1]){

                        chdir(args[1]);
                        char *cwd = getcwd(buf, MAX_BUFFER);
                        setenv("PWD", cwd, 1);
                    }
                    fprintf(stdout, "%s\n", getcwd(buf, MAX_BUFFER));
                    
                    continue;
                }
                /*
                */
                // check if file in args
                // check if input
                // check if output
                // check if append

                if (valid){
                    fork_exec(args);
                }
                /* else pass command onto OS (or in this instance, print them out) */
                else{

                    arg = args;
                    while (*arg) {
                        fprintf(stdout,"%s ",*arg++);
                        fputs ("\n", stdout);
                    }
                }
            }
        }
    }
    return 0; 
}

void syserr(char * msg)   // report error code and abort
{
   fprintf(stderr,"%s: %s", strerror(errno), msg);
   exit(-1);
}

void fork_exec(char **args){

    pid_t p;
    int status;

    switch(p=fork()){

        case -1:
            syserr("fork");

        case 0:
            execvp(args[0], args);

        default:
            waitpid(p, &status, WUNTRACED);
    }
}

void set_shell_env(void){
    
    char *PATH = malloc(MAX_BUFFER*sizeof(char));
    PATH = realpath("./main.c", PATH);            
    setenv("SHELL", PATH, 1);
    PATH = NULL;
    free(PATH);
}