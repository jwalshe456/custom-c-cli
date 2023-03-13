
# myshell

*myshell* is a program which acts as a shell, or command line interpreter. A shell is an interface for users to interact with their machine's OS. *myshell* can handle a number of commands, I/O redirection and background execution.

A terminal and text-editor friendly version of this file can be obtained by opening *manual/readme* or, after running the program, by simply typing:

```sh
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

For detailed descriptions and syntax of each command, type the following:
```sh
help commands
```

## Processes

A process is, loosely put, a program in execution. In actuality, there are multiple parts that make up a process with the program's code being classified as the process's ___text section___. There is also the ___data section___, which contains any global/static variables that exist in the code. Local variables are stored in the process's ___stack___, and its ___heap___ is responsible for dynamic memory allocation.<sup>[1]</sup>  
Processes can be executed in either in the foreground or background.  

Foreground, or interactive<sup>[2]</sup>, processes are those that actually interact with a user.<sup>[3]</sup> By default, the program a user executes will run in the foreground. In a shell environment, if a user runs a program in the foreground, they will have to wait for it to finish before they can run another.

Conversely, if a user wants a process to be run in the background, it will silently run while the user can continue running other processes.<sup>[5]</sup>      
A background process, also known as a batch process or job<sup>[5]</sup>, can be executed by typing "&" after a command, as follows:

```sh
command arg1 &
```

## I/O Redirection

Each file that is open has its own unique ID, known as a ___file descriptor___, or ___fd___ for short. 3 files are opened automatically when a program runs:

* stdin, fd 0
* stdout, fd 1
* stderr, fd 2

A process will always take input from the file with fd 0 (input stream) and its output will go to the file with fd 1 (output stream).<sup>[4]</sup> However, a user can change what file has these fds. This is called ___I/O redirection___.
An example of I/O redirection is as follows:

```sh
file arg1 arg2 < input.txt > output.txt
```  

Here, ___input.txt___ and ___output.txt___ will be the input and output stream, respectively, for ___file___ for the duration of the command's execution.

### Batch Files

Another example of I/O redirection is the use of a batch file. This is a script containing commands to be run by a shell.

```sh
echo hello
cd
ls -a > home_files.txt

```

### Piping

Piping is another form of I/O redirection. With piping, the first program's output stream is the same as the second's input stream i.e., fd 0 and fd 1 are for the same file.<sup>[4]</sup>

```sh
program1 arg1 arg2 | program2
```

## SHELL ENVIRONMENT

Processes can access a special list of variables known as the environment vector, which contains name=value pairs.<sup>[6]</sup> An example of an environment variable is the current or previous working directory, or the home directory.<sup>[7]</sup> These pairs can be modified or new ones can be added by a process, but these changes won't persist after the program exits. This is because "the environment is not held in kernel memory but is stored in the process’s own user-mode address space as the first datum at the top of the process’s stack".<sup>[6]</sup>

## STUDENT DETAILS

___Student Name:___ Jed Walshe  
___Student Number:___ 21338606  
I acknowledge DCU's Academic Integrity Policy. All code and text submitted is my own, unless stated otherwise, in such case it is properly referenced.

## ACKNOWLEDGEMENT

[1] - Silberschatz, Gagne and Galvin (2013), "Processes", in *Operating System Concepts*, 9th edition, ch. 3, pp. 105-107 (Hardback), 129-131 (PDF). PDF available [here](https://drive.uqu.edu.sa/_/mskhayat/files/MySubjects/2017SS%20Operating%20Systems/Abraham%20Silberschatz-Operating%20System%20Concepts%20(9th,2012_12).pdf)

[2] - Silberschatz, Gagne and Galvin (2013), "CPU Scheduling", in *Operating System Concepts*, 9th edition, ch. 6, pp. 274 (Hardback), 298 (PDF). PDF available [here](https://drive.uqu.edu.sa/_/mskhayat/files/MySubjects/2017SS%20Operating%20Systems/Abraham%20Silberschatz-Operating%20System%20Concepts%20(9th,2012_12).pdf)

[3] - Tanenbaum and Bos (2014), "Processes and Threads", in *Modern Operating Systems*, 4th edition, ch. 2, pp. 88-89 (Hardback and PDF). PDF available [here](https://csc-knu.github.io/sys-prog/books/Andrew%20S.%20Tanenbaum%20-%20Modern%20Operating%20Systems.pdf)  

[4] - Tanenbaum and Bos (2014), "Case Study 1: UNIX, Linux and Android", in *Modern Operating Systems*, 4th edition, ch. 10, pp. 780-783 (Hardback and PDF). PDF available [here](https://csc-knu.github.io/sys-prog/books/Andrew%20S.%20Tanenbaum%20-%20Modern%20Operating%20Systems.pdf)    

[5] - Burgess (2001), "Processes and Threads", in *A Short Introduction to Operating Systems*, ch. 6, pp. 32 (Hardback), 33 (PDF). PDF available [here](https://sistop.gwolf.org/html/biblio/Short_introduction_to_operating_systems_-_Mark_Burgess.pdf)

[6] - Silberschatz, Gagne and Galvin (2013), "The Linux System", in *Operating System Concepts*, 9th edition, ch. 18, pp. 793 (Hardback), 817 (PDF). PDF available [here](https://drive.uqu.edu.sa/_/mskhayat/files/MySubjects/2017SS%20Operating%20Systems/Abraham%20Silberschatz-Operating%20System%20Concepts%20(9th,2012_12).pdf)

[7] - Tanenbaum and Bos (2014), "Introduction", in *Modern Operating Systems*, 4th edition, ch. 1, pp. 56 (Hardback and PDF). PDF available [here](https://csc-knu.github.io/sys-prog/books/Andrew%20S.%20Tanenbaum%20-%20Modern%20Operating%20Systems.pdf)
