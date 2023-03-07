/* 

********************************************************************
Student Name: Jed Walshe
Student Number: 21338606
I acknowledge DCU's Academic Integrity Policy. 
All code and text submitted is my own, unless stated otherwise, in such case it is properly referenced.

********************************************************************

*/

// dependencies for main.c
#include "internal.h"
#include "env.h"

#define MAX_ARGS 64          // max # args
#define SEPARATORS " \t\n"   // token separators


int main (int argc, char **argv) {

    extern char *prompt;
    extern bool paused;
    extern bool no_prompt;
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
        // if batch file is being used, 
        // or if a background process has been started
        // no prompt is needed
        if(!using_batchfile && !no_prompt){
            fputs(prompt, stdout); // write prompt
        }
        no_prompt = false;
        
        // read a line
        if (fgets(buf, MAX_BUFFER, stdin)) {
            
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
            else if (!args[0] && paused) {
                switch_pause();
            }
        }
    }
    return 0; 
}

