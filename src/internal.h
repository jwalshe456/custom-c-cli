/* 

********************************************************************
Student Name: Jed Walshe
Student Number: 21338606
I acknowledge DCU's Academic Integrity Policy. 
All code and text submitted is my own, unless stated otherwise, in such case it is properly referenced.

********************************************************************

*/

// definiton of commands to be called by users

#include <sys/types.h>

// definitions for function used in "myshell",
// implementation found in "internal.c"
int cd(char *dir);
char **print_man(char **options);
char **dir(char **args);
int run_command(char **args);
void print_not_valid(char **args);