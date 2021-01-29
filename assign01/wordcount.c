/*
 * File: wordcount.c
 * YOUR NAME ... YOU NEED TO IMPLEMENT THE FUNCTIONS HERE....
 *
 * ....
 */
#define _CRT_SECURE_NO_WARNINGS 1
#define BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
 // include other standard header files needed 

 // if you like copy your myio.h and myio.c here and use it when reading lines from a file!
int CountWords(char*);
 /*
  * YOUR COMMENTS
  */


int main(int argc, char* argv[])
{
    //Variable Declaration
    int i;
    int exists = 0;
    int notExists = 0;
    int returnStatus = 0;
    int wordCount;
    pid_t pid[argc-1];
    // YOU NEED TO IMPLEMENT THIS + some other functions! 
    //goes through each file name in argv 
    for (i = 1; i < argc; i++) {
        //Creates a child process for each file name and saved the pid in pid[i]
        if ((pid[i - 1] = fork()) == 0) {
            // child process
            // call a function to count the number of words in file argv[i]
            wordCount = CountWords(argv[i]);
            // print the number of words in file argv[i] or error message if file argv[i] does not exist
            if (wordCount == -1) {
                printf("Child process %ld for File_%d - %s: doest not exist\n", (long)getpid(), i, argv[i]);
                return 1;
            }
            else {
                printf("Child process %ld for File_%d - %s: number of words is %d\n", (long)getpid(), i, argv[i], wordCount);
                return 0;
            }
            // return/exit with an exit code of 1 if the file argv[i] does not exist. Otherwise; return 0 indicting that a file is counted sucessfully!
        }
    }
    //loops through the saved pids and checks the return status of each one
    for(i = 0; i < argc - 1; i++) {
        waitpid(-1, &returnStatus, 0);
        //if the return status was 1 the file did not exist
        if (WEXITSTATUS(returnStatus) == 1) {
            notExists++;
        }
        //if the return status was 0 the file did exist
        else if (WEXITSTATUS(returnStatus) == 0)
            exists++;
    }
    //prints how many child processes were created and how many were opened sucessfully or not
    printf("Main process created %d child process to count words in %d files\n", notExists + exists, argc-1);
    printf("%d files have been counted sucessfully!\n", exists);
    printf("%d files did not exist.\n", notExists);
    return 0;
}

int CountWords(char* fileName)
{
    FILE* fileOpen;
    char ch;
    int wordCount = 0;
    int state = 0;
    //opens the file passed from main
    fileOpen = fopen(fileName, "r");
    //if it doesn't exist return -1
    if (fileOpen == NULL)
    {
        wordCount = -1;
        return wordCount;
    }
    //loop through each character until end of file
    while((ch = fgetc(fileOpen)) != EOF) {
        //if character is a ' ', tab, or new line keep the state at 0
        if(ch == ' ' || ch == '\t' || ch == '\n') {
            state = 0;
        }
        //if the state is 0, which means previous character was a ' ', tab, or new line
        //and the next character is not one of those then add one to word count
        else if(state == 0) {
            state = 1;
            wordCount++;
        }
    }
    fclose(fileOpen);
    //return the word count
    return wordCount;
}
