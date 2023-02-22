/* 

strtokeg - skeleton shell using strtok to parse command line
usage:
strtokeg
reads in a line of keyboard input at a time, parsing it into
tokens that are separated by white spaces (set by #define SEPARATORS).
can use redirected input if the first token is a recognised internal command, 
then that command is executed. otherwise the tokens are printed on the display.

internal commands:

dir - outputs contents of specified directory, current directory by default
cd - changes to specified directory, unless none is specified, prints out current directory
environ - prints out all environment variables
clr - clears the screen
echo - prints following statement to stdout
pause - ceases activity of shell until 'Enter' is pressed
help -
quit - exits from the program

********************************************************************
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
void use_batch_mode(char *argv);
void fork_exec(char **args);
void syserr(char * msg);
void set_shell_env(char *program_name);
int run_command(char **args, bool valid);

extern char **environ;
char dir_buf[MAX_BUFFER];
bool paused = false;

int main (int argc, char ** argv)
{

    char buf[MAX_BUFFER];                      // line buffer
    char *args[MAX_ARGS];                     // pointers to arg strings
    char **arg;                               // working pointer thru args
    char *prompt = "~~>";               // shell prompt
    bool valid;
    
    // set the shell env variable to this program
    set_shell_env(argv[0]);
    
    // check for batchfile
    if (argv[1]){

        use_batch_mode(argv[1]);

        return 0;
    }
    /* keep reading input until "quit" command or eof of redirected input */

    while (!feof(stdin)) { 
        /* get command line from input */
        fputs (prompt, stdout); // write prompt

        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);   // tokenise input

            // last entry will be NULL
            while ((*arg++ = strtok(NULL,SEPARATORS)));
            

            if (args[0] && !paused) { // potential command on command line

                // assume command to be invalid, until checked by run_command
                valid = false;

                // if command is invalid, print it out to stdout
                if (run_command(args, valid)!= 0) {
                    arg = args;
                    while (*arg) {
                        fprintf(stdout,"%s ",*arg++);
                        fputs ("\n", stdout);
                    }
                }
            }
            else if (!args[0]) {
                paused = false;
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

void set_shell_env(char *program_name){
    
    char *PATH = malloc(MAX_BUFFER*sizeof(char));
    PATH = realpath(program_name, PATH);            
    setenv("SHELL", PATH, 1);
    PATH = NULL;
    free(PATH);
}

void use_batch_mode(char *argv){
    //read each line of file and execute as normal
}

int run_command(char **args, bool valid){
    //check aliases and validity, make changes if need be
    if (!strcmp(args[0],"quit"))   // "quit" command
        exit(0);                     // break out of 'while' loop
                
    if (!strcmp(args[0],"dir")){   // "dir" alias
        args[0] = "ls";
        valid = true;
    }
     
    if (!strcmp(args[0],"clr")) { // "clear" command
        args[0] = "clear";
        valid = true;  
    }

    if (!strcmp(args[0],"environ")){   // "environ" alias
        args[0] = "env";
        valid = true;
    }
    if (!strcmp(args[0],"echo")){   // "environ" alias
        valid = true;
    }
    if (!strcmp(args[0],"pause")){   // "environ" alias
        paused = true;
        return 0;
    }

    if (!strcmp(args[0],"cd")){   // "cd" command
         char *cwd = getcwd(dir_buf, MAX_BUFFER);
         if (args[1]){
             chdir(args[1]);
             setenv("OLDPWD", cwd, 1);
             cwd = getcwd(dir_buf, MAX_BUFFER);
             
             setenv("PWD", cwd, 1);
        }
        fprintf(stdout, "%s\n", cwd);
        return 0;
    }
                // check if file in args
                // check if input
                // check if output
                // check if append

    //call fork_and_exec if valid
    if (valid){
        fork_exec(args);
        return 0;
    }


    //if not valid return non-zero
    return 1;
}
