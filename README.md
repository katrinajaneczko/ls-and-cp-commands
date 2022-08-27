# Project 0 Spring 2022 Project Description
# Unix Utilities
In this first project of CIS3207, I have built 2 UNIX utilities, simple versions of commonly used commands *ls* and *cp*. They are called, respectively, *tuls* and *tucp* to avoid confusion.

*Project Objectives:*
* Re-familiarize myself with the C programming language 
* Re-familiarize myself with a shell / terminal / command-line of UNIX 
* Learn how UNIX utilities are implemented 

# tuls
The program *tuls* is a simple program. Generally, it lists information about the FILES in a directory. By default, *tuls* lists the files in the current directory. 
To invoke the program the user types:  
prompt> *./tuls*
 >    lscmd.c  
 >    a.out  
 >    ..  
 >    .  
  
or, 

prompt> *./tuls  direc    <*where *direc* is a directory path*>*  
>[[direc]]  
>      ->direc_1_file  
>      ->direc_2_file  
>local_1_file  
>local_2_file  
>local_3_file  
>..  
>.  
 
As shown, *tuls* prints out the contents of the specified directory. When there is no additional parameter for the *tuls* command, just the list of the files in the current directory is produced. When a directory path is included with the *tuls* command, then the *tuls* command prints the list of files in the current directory and *recursively* prints the list of files in all directories specified by the directory path argument. 

To create the *tuls* executable, I created a single source file, *tuls.c*, and wrote a little C code to implement this simplified version of *tuls*. To compile this program, do the following: 

prompt> gcc -o tuls tuls.c -Wall -Werror 
prompt>  

This will make a single **executable binary** called *tuls* which can then be run as above. 

In this process, I learned how to use a few library routines from the C standard library (often called *libc*) to implement the source code for this program, which we’ll assume is in a file called *tuls.c*. 

**Details**
* The program *tuls* can be invoked with zero or one argument on the command line; with 1 argument, recursively print the entries in the directories in the local directory as well as the files in the local directry.
* In all non-error cases, *tuls* exits with status code 0, usually by returning a 0 from *main()* (or by calling *exit(0)*).
* If /arguments/ are specified on the command line, *tuls* just prints the names of the entries (files and directories) in the current directory. 
* If the program tries and fails to *open or scan a directory*, it prints the message “tuls: cannot open directory” (followed by a newline) and exits with status code 1. 

# tucp
The second utility built is called **tucp**, a variant of the UNIX tool *cp*. In this project, this command is used to copy files or a group of files. It creates an exact image (copy) of a file on a disk. The *tucp* command requires at least two filenames in its arguments.  
The following are the possible command usages: 

*tucp* Sourcefile Destinationfile 
*tucp* Sourcefile Directory 
*tucp* Sourcefile-1 Sourcefile-2 Sourcefile-3 Sourcefile-n Directory 

# Details
* If the *tucp* command contains two file names, then it copies the contents of the 1st file to the 2nd file. If the 2nd file doesn’t exist, then it first creates a file with the specified name and the content of the first file is copied to it. But if the second file already exists, then it is simply overwritten without any warning. 
* If the *tucp* command has one or more arguments, specifying source file names and following those arguments, an argument specifying a directory name, then the command copies each source file to the destination directory with the same source file name. The destination file is created if did not exist, or overwritten if it already existed.

# Grading Rubric for Project 0 – Total 10 Points   
## Part A 5 Points   
* tuls – correctly uses the library functions scandir(), opendir(), readdir()  (1 pt) 
* tuls – correctly identifies directories vs files (0.5 pts) 
* tuls - correctly lists the current directory contents (1.5 pt)     
* tuls - correctly lists the current directory and recursively lists directory contents from the current directory (2 pts)     

## Part B 5 Points 
* tucp – correctly parses the command line into source and destination components (1 pt) 
* tucp - correctly copies 1 source file to a destination file (1 pt)    
* tucp – correctly copies a source file to a specified directory (1 pt)    
* tucp - correctly copies multiple source files to a specified directory. (2 pt)  
