/* 

********************************************************************
Student Name: Jed Walshe
Student Number: 21338606
I acknowledge DCU's Academic Integrity Policy. 
All code and text submitted is my own, unless stated otherwise, in such case it is properly referenced.

********************************************************************

*/

// definiton of commands to be called by users
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>

// definitions for function used in "myshell",
// implementation found in "built-in.c"

/* BUILT-INS */
void env(void);
void clr(void);
void switch_pause(void);
void quit(void);

void cd(char **args);

void help(char **options, int len);
void dir(char **args, int len);
void echo(char **args, int len);

/* checking of commands */
int run_command(char **args, int len);
void print_not_valid(char **args, int len);
bool built_in(char **args, int len);
