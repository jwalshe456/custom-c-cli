// definiton of commands to be called by users

#include <sys/types.h>

// definitions for function used in "myshell",
// implementation found in "internal.c"
int cd(char *dir);
char **print_man(char **options);
char **dir(char **args);
int run_command(char **args);
void print_not_valid(char **args);