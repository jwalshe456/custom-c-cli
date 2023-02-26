# myshell

*myshell* is a program which acts as a shell, or command line interpreter. A shell is an interface for users to interact with their machine's OS. *myshell* can handle a number of commands, I/O redirection and background execution.

A seperate text file containing much of the following information can be obtained after running the program by simply typing:
```
help
```  

## Supported Commands

UNIX commands are mostly supported, though the following commands act as aliases or differ in function from their standards:

* ___dir___ - outputs contents of specified directory, current directory by default  
* ___cd___ - changes to specified directory, unless none is specified, prints out current directory  
* ___environ___ - prints out all environment variables  
* ___clr___ - clears the screen  
* ___echo___ - prints following statement to stdout  
* ___pause___ - ceases activity of shell until 'Enter' is pressed  
* ___help___ - display manual pages  
* ___quit___ - exits from the program  

For detailed descriptions and syntax of each command type after starting the program:
``` 
help commands
```

## I/O Redirection

I/O redirection is the changing of the input and/or output stream(s) for a file. By default these are ___stdin___ and ___stdout___.  
An example of I/O redirection is as follows:
```
file arg1 arg2 < input.txt > output.txt
```  
Here, ___input.txt___ and ___output.txt___ will act as the input and output stream for ___file___ for the duration of the command's execution.
Another example includes the use of a batchfile, a file containing commands to be run, and acts as the input stream for the shell:

```
myshell batchfile

```

## Background Execution

Background processes run without any interaction on the users part. This differs from foreground processes.  
A background process can be executed by invoking "&" after a command, as follows:
```
command args &
```

## Shell


## STUDENT DETAILS



## ACKNOWLEDGEMENT
