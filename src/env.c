/* 

********************************************************************
Student Name: Jed Walshe
Student Number: 21338606
I acknowledge DCU's Academic Integrity Policy. 
All code and text submitted is my own, unless stated otherwise, in such case it is properly referenced.

********************************************************************

*/

// implementations of functions which alter environment variables and prompt
// contains main built-in libraries to support shell as well as key definitions.

#include "env.h"

char dir_buf[MAX_BUFFER]; // buffer to store current directory
char *prompt; // global var for command prompt
bool no_prompt;

static bool dont_wait = false;           // flag for background execution, false by default

/* 
function: set_shell_env
arguments: char* "file_name": file to be added to environment variables, 
    char* "env_name": name of environment variable to be changed 
description: sets environment variable to path of a specified file/directory
returns: void
*/

void set_shell_env(char *file_name, char *env_name){ //set SHELL environment variable to program
    
    char path[MAX_BUFFER]; 
    // get path to file

    realpath(file_name, path);
    if(path == NULL){

        syserr("path to file couldn't be allocated to memory");
    }
    
    // set SHELL to path to myshell
    if(setenv(env_name, path, 1)){

        syserr("environment variable couldn't be set");
    }
    

}

/* 
function: get_prompt
arguments: void 
description: gets current directory and appends prompt string to it
returns: char*, current directory
*/

char *get_prompt(void){ // get the current directory and update the prompt
    char prompt[] = " >>> "; 
    char *dir = getcwd(dir_buf, MAX_BUFFER);
    strcat(dir, prompt);
    return dir;
}

/* 
function: syserr
arguments: char* "msg": message to be printed in case of error 
description: prints error message and aborts shell
returns: void
*/

void syserr(char * msg)
{
   fprintf(stderr,"%s: %s", strerror(errno), msg);
   exit(-1);
}

/* 
function: fork_exec
arguments: char** "args": a list of arguments
description: handles forking and executing of commands
returns: void
*/
void fork_exec(char **args){

    pid_t p;
    int status;

    // run commands as child process
    switch(p=fork()){

        case -1:
            syserr("bad fork");

        case 0:

            if(dont_wait){
                printf("Process %d running in background...\n", getpid());
                fputs(prompt, stdout);
                fflush(stdout);
            }
            
            execvp(args[0], args);

        default:
            //check if shell should wait for process to finish
            if(!dont_wait){
                waitpid(p, &status, WUNTRACED);
            }

            else{
                dont_wait = false;
                no_prompt = true;
                // prevents creation of zombie processes
                signal(SIGCHLD, SIG_IGN);
            }

    }
}

/* 
function: check_file_type
arguments: char* "file": filename
description: check to see if file is a directory or executable
returns: int, denotes success
*/

int check_file_type(char* file){
    // get file mode
    struct stat stats = {0};
    stat(file, &stats);
    int file_mode = stats.st_mode;

    // check if directory
    if (file_mode & S_IFDIR){
        return IS_DIR;
    }

    // check if executable
    if (file_mode & S_IFREG && file_mode & S_IXUSR){
        return IS_EXEC;
    }

    return -1;
}

/* 
function: set_io_stream
arguments: char* "file": filename,
    int "flags": specifies permissions when opening file,
    int "stream_fd": file descriptor for stream to be replaced
description: switches stdin/stdout with specified file
returns: void
*/

void set_io_stream(char *file, int flags, int stream_fd){
    
    int fd = open(file, flags, 0777);
    if (fd == -1){
        syserr("failed to open file\n");
    }
    dup2(fd, stream_fd);
    close(fd);
}

/* 
function: parse
arguments: char** "args": a list of arguments
description: parse arguments, looking for intent of i/o redirection
returns: char**, list of cleaned arguments
*/

// based from approach seen at;
// https://stackoverflow.com/questions/52939356/redirecting-i-o-in-a-custom-shell-program-written-in-c

char **parse(char **args, int len){
    
    bool do_shift = false;


    // assume stdin will be changed by default
    int flags = O_RDONLY, stream_fd = STDIN_FILENO;

    // find intent of i/o redirection
    int i = 0, j = 0;
    char *arg = args[0];
    while(i < len){
        // change stdin
        if(arg[0] == '<'){
            do_shift = true;
    
        }

        // change stdout
        else if(arg[0] == '>'){
            stream_fd = STDOUT_FILENO;
            flags = O_WRONLY|O_CREAT;
            do_shift = true;
            
            // overwrite file
            if (!arg[1]){
                flags |= O_TRUNC;
            }

            // append to file
            else if(arg[1] == '>'){
                flags |= O_APPEND;
            }

            // print error message if word begins with > e.g., >myfile
            else{
                fprintf(stdout, "bad file name: %s\n", arg);
                exit(1);
            }
        }

        // background process invocation
        else if (arg[0] == '&'){
            dont_wait = true;
            args[i] = NULL;
        }
        
        // change i/o stream
        if (do_shift){
            // set position to start cleaning arguments
            if (j == 0){
                j = i;
            }
            set_io_stream(args[++i], flags, stream_fd);
        }
            

        arg = args[++i];
    }

    // clean command line by removing arrows and input/output files
    if (do_shift){
        int k = j;
        while(args[k]){
            args[k] = NULL;
            ++k;
        }
    }

    return args;
}
