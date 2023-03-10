/* 

********************************************************************
Student Name: Jed Walshe
Student Number: 21338606
I acknowledge DCU's Academic Integrity Policy. 
All code and text submitted is my own, unless stated otherwise, in such case it is properly referenced.

********************************************************************

*/

// dependencies for main.c
#include "built-in.h"
#include "env.h"

#define MAX_ARGS 64          // max # args
#define SEPARATORS " \t\n"   // token separators


int main (int argc, char **argv) {

    extern char *prompt;
    extern bool paused;
    extern bool no_prompt;
    char buf[MAX_BUFFER];    // line buffer
    char *args[MAX_ARGS];    // pointers to arg strings
    char **arg;              // working pointer thru args
    char *env_vars[] = {"SHELL", "./manual", "MAN_DIR"};
    int length;
    // set the shell env variable to this program
    set_shell_env(argv[0], env_vars[0]);

    // set absolute URL for directory containing man pages
    set_shell_env(env_vars[1], env_vars[2]);

    prompt = get_prompt();
    
    // check for batchfile, if absent prompt user for input
    if (argv[1]) {
        set_io_stream(argv[1], O_RDONLY, STDIN_FILENO);

    }

    // keep reading input until "quit" command or eof of redirected input
    while (!feof(stdin)) { 
        // if batch file is being used, 
        // or if a background process has been started
        // or if shell is paused
        // no prompt is needed
        if(!argv[1] && !no_prompt && !paused){
            fputs(prompt, stdout); // write prompt
        }
        no_prompt = false;
        
        // read a line
        if (fgets(buf, MAX_BUFFER, stdin)) {
            length = 0;
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);
            length++;

            while ((*arg++ = strtok(NULL,SEPARATORS))){length++;}

            // potential command on command line, run if not paused
            if (args[0] && !paused) {

                // if command fails to run, print it out to stdout
                run_command(args, length);
            }


            // if enter key is pressed, unpause shell, does nothing if already unpaused
            else if (!args[0] && paused) {
                switch_pause();
            }
        }
    }
    return 0; 
}

