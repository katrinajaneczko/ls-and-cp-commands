// Katrina Janeczko
// 3207.003
// 1-19-2022
// Project 0: Implementing Two Unix/Linux Utilities Part A

/* tuls prints out the contents of the specified directory. 
When there is no additional parameter for the tuls command, 
just the list of the files in the current directory is produced. 
When a directory path is included with the tuls command, 
then the tuls command prints the list of files in the current directory 
and recursively prints the list of files in all directories 
specified by the directory path argument. 

Note: To compile: gcc -o tuls tuls.c -Wall -Werror
As test: name = "/Users/katrinajaneczko/Desktop/DSA"
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

void listDirs(const char *name, int indent);

int main(int argc, char* argv[]){
    struct stat buf;
    char* name; 

    if (!(argc == 2 || argc == 1)) {
        printf("Incorrect argument amount");
        exit(EXIT_FAILURE);
    }
    else if (argc == 2) { 
        name = argv[1]; //if user enters ./tuls direcpath, then name = direcpath 
    }
    else {
        name = "."; //if no args entered besides .\tuls, then name = "." (current directory)
    }
    
    //CHECK IF WHAT THE USER ORIGINALLY ENTERED IS A DIRECTORY
    //if stat doesn't work; error
    if (stat(name, &buf) == -1) { 
        perror("stat");
        printf("%s", "tuls: cannot open directory\n");
        exit(EXIT_FAILURE);
    }
    //the argument the user entered is not a directory
    else if (!S_ISDIR(buf.st_mode)) { 
        printf("%s", "tuls: cannot open directory\n");
        exit(EXIT_FAILURE);
    }
    //the argument is a directory
    else {
        //if 1 argument, also display current (working) directory
        if (argc == 2) {
            printf("[Current (working) directory]\n");
            listDirs(".", 2);
            //call the function on specified directory to list directories
            printf("\n[%s]\n", name);
            listDirs(name, 2);
        }
        else { //0 arguments, print only wokring directory
            printf("[Current (working) directory]\n");
            listDirs(".", 2);
        }
        exit(EXIT_SUCCESS);
    }
    
}


void listDirs(const char *name, int indent) { 
        struct stat buf;
        struct dirent *dirent; //dirent is directory entry
        DIR *dir = opendir(name);  //assign dir to opendir(name) [directory stream] 

        //OPEN DIRECTORY
        //check if directory stream dir is unable to open; if (!dir) means if (dir==NULL)
        if (dir == NULL) {
            printf("%s", "tuls: cannot open directory\n");
            exit(EXIT_FAILURE);
        }

        //else it is able to open the directory, so read it
        //READ DIRECTORY -> WHILE THERE ARE MORE ENTRIES
        while ( (dirent = readdir(dir)) != NULL) { //readdir returns pointer to next dirent struct entry in directory stream dir
        
            //ignore . or .. to avoud printing their contents recursively
            if ( (strcmp(dirent->d_name,".") != 0)  &&  (strcmp(dirent->d_name,"..") != 0)  &&  (strcmp(dirent->d_name,".DS_Store") != 0) ) {
                //build new path name for this entry with dirent->d_name concatenated
                char subName[10000];
                strcpy(subName, name);
                strcat(subName, "/");
                strcat(subName, dirent->d_name);

                //CHECK IF THIS ENTRY IS A DIRECTORY OR NOT USING STAT()
                //if doesn't work, stat gives -1
                if (stat(subName, &buf) == -1) { //set buf to name and check if works or returns -1
                    perror("stat");
                    exit(EXIT_FAILURE);
                }

                //print indent(s) corresponding to level
                for (int i=0; i<indent; ++i) {
                    printf(" ");
                }

                //IF NOT DIRECTORY, JUST PRINT NAME
                if (S_ISDIR(buf.st_mode) == 0) { //S_ISDIR macro returns nonzero if file is directory. 0 means false/is not directory
                    //just print the entry
                    printf("%s\n", dirent->d_name);
                }

                //IF YES DIRECTORY, MAKE RECURSIVE CALL ON ENTRY
                if (S_ISDIR(buf.st_mode) != 0) { 

                    printf("%s\n", dirent->d_name);

                    //make recursive call
                    listDirs(subName, indent+2);
            }
        }    
    }
    closedir(dir);
} 