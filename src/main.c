/* 

********************************************************************
myshell -  main program to run shell
    based off of example code by Ian G Graham

internal commands:

dir - outputs contents of specified directory, current directory by default
cd - changes to specified directory, unless none is specified, prints out current directory
environ - prints out all environment variables
clr - clears the screen
echo - prints following statement to stdout
pause - ceases activity of shell until 'Enter' is pressed
help - display manual pages
quit - exits from the program

********************************************************************

*/

// dependencies for main.c
#include "internal.h"
#include "env.h"

#define MAX_ARGS 64          // max # args
#define SEPARATORS " \t\n"   // token separators

extern char *prompt;

int main (int argc, char **argv) {

    extern bool paused;
    bool using_batchfile;
    char buf[MAX_BUFFER];    // line buffer
    char *args[MAX_ARGS];    // pointers to arg strings
    char **arg;              // working pointer thru args
    
    // set the shell env variable to this program
    set_shell_env(argv[0], "SHELL");

    // set absolute URL for directory containing man pages
    set_shell_env("./manual", "MAN_DIR");

    prompt = get_prompt();
    
    // check for batchfile, if absent prompt user for input
    if (argv[1]) {
        set_io_stream(argv[1], O_RDONLY, STDIN_FILENO);
        using_batchfile = true;

    }

    // keep reading input until "quit" command or eof of redirected input
    while (!feof(stdin)) { 
        // get command line from input
        if(!using_batchfile){

            fputs(prompt, stdout); // write prompt
        }
        
        // read a line
        if (fgets(buf, MAX_BUFFER, stdin )) {
            
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);

            while ((*arg++ = strtok(NULL,SEPARATORS)));

            // potential command on command line, run if not paused
            if (args[0] && !paused) {

                // if command fails to run, print it out to stdout
                if (run_command(args) != 0) {
                    print_not_valid(args);   
                }
            }

            // if enter key is pressed, unpause shell, does nothing if already unpaused
            else if (!args[0]) {
                paused = false;
            }
        }
    }
    return 0; 
}

