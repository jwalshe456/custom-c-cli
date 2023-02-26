// definitions of functions which are handle the "under-the-hood" operation of the shell
// and any changes made to it and the environment variables

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>


#define IS_DIR 0          // directory descriptor
#define IS_EXEC 1         // executable descriptor
#define MAX_BUFFER 1024   // max line buffer

// definitions for function used in "myshell",
// implementation found in "env.c"
void set_shell_env(char *file_name, char *env_name);
char *get_prompt(void);
void syserr(char * msg);
void fork_exec(char **args);
int check_file_type(char* file);
void set_io_stream(char *file, int flags, int stream_fd);
int parse_run(char **args);
