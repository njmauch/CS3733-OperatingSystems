/*
 * File: driver2.c
 * YOUR NAME ... YOU NEED TO IMPLEMENT THE main() + additional functions if needed
 *
 * ....
 */
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include "myio.h"

 /*
  * Second one (say driver2.c) gets two command-line arguments:
  * driver2 input_file.txt output_file.txt
  * input_file.txt is a text file containig many lines.
  *
  * Your program reads each line from the input file and removes
  * the extra space characters between the words in that line.
  * Then it prints the new line into output_file.txt.
  * So there will be at most one space character between the words in
  * each line of output_file.txt.
  * Remove all the space characters from the begining and end of the string as well.
  * Leave only one space character between the words.
  * Use only the space character as a seperator, so any other sequence of charcters
  * are treated like a word (e.g., "   aa    3c   %$3a    " has three words,
  * after removing the extra space characters you should have "aa 3c %$3a")
  */

int main(int argc, char* argv[])
{
    // YOU NEED TO IMPLEMENT THIS (USE FUNCTIONS FROM myio.h) 
    // IF NEEDED, YOU CAN ALSO IMLEMENT YOUR OWN FUNCTIONS HERE
    FILE* fileRead;
    FILE* fileWrite;
    char* aLine;
    char* token;

    //Ensures that the correct number of command arguments are entered into the command line
    if (argc != 3) {
        fprintf(stderr, "%s", "Invalid input parameters. Usage: (program) (input_file) (output_file)\n");
        exit(1);
    }
    //Opens the input file for read only
    fileRead = fopen(argv[1], "r");
    if (fileRead == NULL) {
        fprintf(stderr, "%s", "Error opening file\n");
        exit(1);
    }
    //Opens the write file to append the tokens that will be given
    fileWrite = fopen(argv[2], "a+");
    if (fileWrite == NULL) {
        fprintf(stderr, "%s", "Error opening file\n");
        exit(1);
    }
    //Loops through the returning string for each word in the string
    while ((aLine = ReadLineFile(fileRead)) != NULL) {
        //Gets the first word in the string
        token = strtok(aLine, " \t");
        //If null break out, if not write that word to the file with a " " at the end
        while (token != NULL) {
            fprintf(fileWrite, "%s ", token);
            //get next word
            token = strtok(NULL, " ");
        }
        //Insert a new line into the file
        fprintf(fileWrite, "\n");
    }
    //Close the files
    fclose(fileRead);
    fclose(fileWrite);
    return 0;
}
