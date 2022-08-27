// Katrina Janeczko
// 3207.003
// 1-26-2022
// Project 0: Implementing Two Unix/Linux Utilities Part B

/* tucp, a variant of the UNIX tool cp is used to copy files or a group of files. 
It creates an exact image (copy) of a file on a disk. 
The tucp command requires at least two filenames in its arguments.
Examples:
1) tucp Sourcefile Destinationfile
    -> If the tucp command contains two file names, then it copies the contents of the 
    1st file to the 2nd file. If the 2nd file doesn’t exist, then it first creates a file 
    with the specified name and the content of the first file is copied to it. But if the 
    second file already exists, then it is simply overwritten without any warning.
2) tucp Sourcefile Directory
3) tucp Sourcefile-1 Sourcefile-2 Sourcefile-3 Sourcefile-n Directory
    If the tucp command has one or more arguments, specifying source file names and 
    following those arguments, an argument specifying a directory name, then the command 
    copies each source file to the destination directory with the same source file name. 
    The destination file is created if did not exist, or overwritten if it already existed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

void copyFileToFile(const char *source_file, const char *target_file);
void copyFileToDirectory(const char *source_file, const char *target_directory);
void copyNFilesToDirectory(char **source_files, int n, const char *target_directory);

int main(int argc, char* argv[]) {

    //Test num args: if 1 or 2, error because not enough args
    if (argc < 3) {
        printf("tucp: incorrect arg amount");
        exit(EXIT_FAILURE);
    }
    //Test num args: if 3, means either case 1 (file to file) or case 2 (file to directory)
    else if (argc == 3) {
        struct stat buf1, buf2;
        //Test if arg[1] exists
        if ( (stat(argv[1], &buf1) != 0) ) { //0 means exists; if returns -1, means didn't work
            printf("tucp: cannot copy file\n");
            exit(EXIT_FAILURE);
        }

        //Test st_mode of arg[1]: if not file, throw error incorrect arg
        if (!S_ISREG(buf1.st_mode)) { 
            printf("tucp: incorrect first argument\n");
            exit(EXIT_FAILURE);
        }

        //Test if arg[2] exists
        if ( (stat(argv[2], &buf2) == 0) ) {  
            //Test st_mode of arg[2]: if exists but not file and not directory, throw error incorrect arg
            if ( (!S_ISREG(buf2.st_mode)) && (!S_ISDIR(buf2.st_mode)) ) { 
                printf("tucp: incorrect second argument\n");
                exit(EXIT_FAILURE);
            }

            //Test st_mode of arg[2]: if exists and is directory, case 2 (file to directory)
            else if (S_ISDIR(buf2.st_mode)) {
                //CASE 2: tucp Sourcefile Directory
                copyFileToDirectory(argv[1], argv[2]); //source_file=argv[1], target_directory=argv[2]
                exit(EXIT_SUCCESS);
            } 
        }
        //At this point, either arg[2] doesn't exist OR exists and is regular file
        //CASE 1: tucp Sourcefile Destinationfile
        copyFileToFile(argv[1], argv[2]); //source_file=argv[1], target_file=argv[2]
        
    }
    //Test num args: if 4+, means case 3 (n files to directory)
    else {
        struct stat bufdir;
        //create array of pointers to chat (strings), where each entry is a source file name
        char *source_files[argc - 2];
        for (int i = 1; i < argc - 1; ++i) {
            struct stat buf;
            //Test if stat works for the n files
            if ( (stat(argv[i], &buf) == -1) ) { 
                perror("stat");
                printf("tucp: cannot copy file\n");
                exit(EXIT_FAILURE);
            }

            //Test if each argv[i] is indeed a regular file:
            if (!S_ISREG(buf.st_mode)) { 
                printf("tucp: incorrect argument\n");
                exit(EXIT_FAILURE);
            }

            //source_files[i-1] = (char*) malloc(256 * sizeof(argv[i]));

            //place it into array
            source_files[i-1] = argv[i];
        }

        //Test if stat works for the directory specified
        if ( (stat(argv[argc-1], &bufdir) == -1) ) { 
            perror("stat");
            printf("tucp: cannot copy file\n");
            exit(EXIT_FAILURE);
        }

        //Test if argv[argc-1] is indeed a directory
        if (!S_ISDIR(bufdir.st_mode)) { 
            printf("tucp: incorrect argument\n");
            exit(EXIT_FAILURE);
        }
        
        //CASE 3: tucp Sourcefile-1 Sourcefile-2 Sourcefile-3 Sourcefile-n Directory
        copyNFilesToDirectory(source_files, argc-2, argv[argc-1]); //target_directory=argv[argc-1] bc last arg; n=argc-2 bc last arg and first arg aren't files
    }
   
     exit(EXIT_SUCCESS);
}

void copyFileToFile(const char *source_file, const char *target_file) {
    FILE *source;
    FILE *target;

    //open source file, and set mode to read
    source = fopen(source_file, "r");

    //check if source file exists
    if (source == NULL) {
        perror("fopen");
        printf("tucp: source file is null\n");
        fclose(source);
        exit(EXIT_FAILURE);
    }

    //open target file, and set mode to write
    target = fopen(target_file, "w");

    //check that opening target file worked
    if (target == NULL) {
        perror("fopen");
        printf("tucp: target file is null\n");
        fclose(source);
        fclose(target);
        exit(EXIT_FAILURE);
    }

    //if the target file already exists, then overwrite without warning
    //if target file doesn't exist, then create new file with specified name and copy contents to it
    //note: fopen(__, "w") automatically knows if exists or not, & will either create new file with specified name or open already existing file
    char buffer[256];
    size_t size = 0; //size in bytes
    while ((size = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, size, target) != size) {
            perror("fwrite");
            exit(EXIT_FAILURE);
        }
    }   
    
    //this only works for  text file
    /*char c;
    while ((c = fgetc(source)) != EOF) {
        fputc(c, target);
    }*/

    printf("File %s copied successfully to file %s\n", source_file, target_file);
    fclose(source);
    fclose(target);
}

void copyFileToDirectory(const char *source_file, const char *target_directory) {
    DIR *dir;
    char new_file[10000];
    
    //check if target_directory is able to be opened and open, assign directory stream to dir
    if ( (dir = opendir(target_directory)) == NULL) {
        perror("opendir");
        printf("tucp: cannot open directory\n");
        exit(EXIT_FAILURE);
    }
    
    //create new file (call it new_file) in target directory with same name as source_file
    //need to copy & concatenate to get pathname for new_file
    strcpy(new_file, target_directory);
    strcat(new_file, "/");
    strcat(new_file, source_file);
    
    //copy source_file into new_file, where new_file is in target directory
    copyFileToFile(source_file, new_file);

    printf("File %s copied successfully to directory %s\n", source_file, target_directory);
}

void copyNFilesToDirectory(char **source_files, int n, const char *target_directory) {
    for (int i = 0; i < n; ++i) {
        copyFileToDirectory(source_files[i], target_directory);
    }
    printf("All files copied successfully to directory %s\n", target_directory);
}
        