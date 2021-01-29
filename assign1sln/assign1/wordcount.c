/*
 * File: wordcount.c
 * YOUR NAME ... YOU NEED TO IMPLEMENT THE FUNCTIONS HERE....
 *
 * ....
 */
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
 // include other standard header files needed 

 // if you like copy your myio.h and myio.c here and use it when reading lines from a file!
#include "myio.h"  

/*
 * YOUR COMMENTS
 */


int main(int argc, char* argv[])
{
    int i;
    int exists = 0;
    int notExists = 0;
    int returnStatus = 0;
    pid_t wpid;
    int wordCount;
    // YOU NEED TO IMPLEMENT THIS + some other functions! 
    for (i = 1; i < argc; i++) {
        if (fork() == 0) {
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
    while ((wpid = wait(&returnStatus)) != -1) {
        if (returnStatus == 1)
            notExists++;
        if (returnStatus == 0)
            exists++;
    }
    printf("Main process created %d child process to count words in %d files\n", notExists + exists, argc - 1);
    printf("%d files have been counted sucessfully!\n", exists);
    printf("%d files did not exist.\n", notExists);
    return 0;
}

int CountWords(char* fileName)
{
    FILE* fileOpen;
    char ch;
    int wordCount = 0;

    fileOpen = fopen(fileName, "r");
    if (fileOpen == NULL)
    {
        wordCount = -1;
        return wordCount;
    }
    while ((ch = fgetc(fileOpen)) != EOF) {
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            wordCount += 1;
        }
    }
    fclose(fileOpen);
    return wordCount;
}
