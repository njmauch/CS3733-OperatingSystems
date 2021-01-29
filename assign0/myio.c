/*
 * File: myio.c
 * YOUR NAME ...
 * YOU NEED TO IMPLEMENT THE FUNCTIONS  here
 * ....
 */
#define _CRT_SECURE_NO_WARNINGS 1
#define BUFFER_SIZE 100

#include <stdio.h>
#include <stdlib.h>
#include "myio.h"
#include <limits.h>
#include <string.h>


 /*
  * Function: ReadInteger
  * Usage: i = ReadInteger();
  * ------------------------
  * ReadInteger reads a line of text from standard input and scans
  * it as an integer. To read the line, you can call the ReadLine()
  * function that is described below. If all the characters in the
  * given line are digits, then convert it to an integer and
  * return it. The very first character could be the sign character
  * '-' or '+'.
  * Otherwise, if the given string contains any other character,
  * this function will give an error meassage and keep asking user
  * to enter a valid sequnece of characters that can be converted to
  * an integer.
  * The followings are valid integers: 234, -345, +453.
  * The followings are not valid integers: 34  5, 3afg, 3-3, --45.
  */
int ReadInteger(void)
{
    // YOU NEED TO IMPLEMENT
    char* psbInput;
    int iValue, i;
    int boolRun = 0;

    //boolRun through this loop as long as there is a character in the line input by user
    do {
        boolRun = 0;
        printf("Enter an integer:");
        //reads the line from user
        psbInput = ReadLine();
        //loop through each character in the input
        for (i = 0; i < INT_MAX; i++) {
            //If the first character is a + or - continue to next character
            if (i == 0) {
                if (psbInput[i] == '-' || psbInput[i] == '+') {
                    continue;
                }
            }
            //If end of input break out of loop
            if (psbInput[i] == '\0') {
                break;
            }
            //If the character isn't in the range of 0-9 values display error to screen
            else if (psbInput[i] > '9' || psbInput[i] < '0') {
                boolRun = 1;
                fprintf(stderr, "%s", "You entered an invalid integer\n");
                break;
            }
        }
    } while (boolRun == 1);
    //convert the string into an integer
    iValue = atoi(psbInput);
    return iValue;
}



/*
 * Function: ReadDouble
 * Usage: x = ReadDouble();
 * ---------------------
 * ReadDouble reads a line of text from standard input and scans
 * it as a double.  As described above, you can call ReadLine() to
 * read the line. If the number cannot be scanned as double or if
 * extra characters follow after the number ends, this function
 * gives an error and keep asking user to enter a valid double.
 * The followings are valid doubless: 24, -35, +43, -23.54, +45.3
 * The followings are not valid doubles: 34  5, 3afg, 3.3.3, --45.5
 */
double ReadDouble(void)
{
    // YOU NEED TO IMPLEMENT
    char *psbInput;
    int i, boolDecimal;
    double dValue;
    int boolRun = 0;
    //boolRun through this loop as long as there is a character in the line input by user
    do {
        boolRun = 0;
        boolDecimal = 0;

        printf("Enter a double:");
        //Read the string from the user
        psbInput = ReadLine();
        for (i = 0; i < INT_MAX; i++) {
            //if the first character of the string is a + or - continue to next character
            if (i == 0) {
                if (psbInput[i] == '-' || psbInput[i] == '+') {
                    continue;
                }
            }
            //break out of loop if end of string
            if (psbInput[i] == '\0') {
                break;
            }
            //Keep track if boolDecimal value for double for only one boolDecimal
            else if (psbInput[i] == '.' && boolDecimal == 0) {
                boolDecimal = 1;
            }
            //check if character is in the valid range of 0-9 values, if not let user know
            else if (psbInput[i] > '9' || psbInput[i] < '0') {
                boolRun = 1;
                fprintf(stderr, "%s", "You entered an invalid double\n");
                break;
            }
        }
    } while (boolRun == 1);
    //convert the string to a double
    dValue = strtod(psbInput, NULL);
    return dValue;
}



/*
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input stdin and returns
 * the line as a string.  The newline character that terminates
 * the input is not stored as part of the string.
 *
 * Note: this function above ReadLine(); can simply be implemented
 *       by using the next function as follows..
 *       So, focus on the implementation of ReadLineFile(infile); below
 */
char* ReadLine(void)
{
    //Reads in the line from stdinput user enters
    char* psbInputBuffer;
    psbInputBuffer = calloc(CHAR_MAX, sizeof(char));
    fgets(psbInputBuffer, CHAR_MAX, stdin);
    strtok(psbInputBuffer, "\n");
    return psbInputBuffer;
}



/*
 * Function: ReadLineFile
 * Usage: s = ReadLineFile(infile);
 * ----------------------------
 * ReadLineFile reads a line of text from the input file which
 * is already open and pointed by infile. It then reads the line,
 * dynamically allocates space, and returns the line as a string.
 * The newline character that terminates the input is not stored
 * as part of the string.
 * The ReadLineFile function returns NULL if infile is at the
 * end-of-file position.
 */
char* ReadLineFile(FILE* infile)
{
    // YOU NEED TO IMPLEMENT. 
    // Note: infile is already opened by the driver programa so don't try to open it here
    char *szInputBuffer;
    char *szDynamicLine;
    int iBufferSize = BUFFER_SIZE;
    int i, j;

    //Allocate the input buffer with size 100
    szInputBuffer = calloc(iBufferSize, (iBufferSize) * sizeof(char*));

    //If Unable to allocate give error
    if (szInputBuffer == NULL) {
        fprintf(stderr, "%s", "Out of memory");
    }

    //Get the first character of the line
    szInputBuffer[0] = fgetc(infile);

    //If null of end of file return null to main
    if (szInputBuffer[0] == EOF) {
        return NULL;
    }

    //Loop through the buffer size to get character by character
    for (i = 1; i < iBufferSize + 1; i++) {
        //get next character of the line
        szInputBuffer[i] = fgetc(infile);
        //If you reach a new line or end of file break out of loop
        if (szInputBuffer[i] == '\n' || szInputBuffer[i] == EOF) {
            break;
        }
        //If you reach the buffer size reallocate memory to double the input buffer
        if (i == iBufferSize) {
            iBufferSize *= 2;
            --i;
            //reallocate the memory for the buffer
            szInputBuffer = (char*)realloc(szInputBuffer, iBufferSize);
            if (szInputBuffer == NULL) {
                fprintf(stderr, "%s", "Out of memory");
            }
        }
    }
    //Since you know the size of the string, create a new string with that size
    szDynamicLine = malloc((i) * sizeof(char*));
    //Insert the input buffer into the new string
    for (j = 0; j < i; j++) {
        szDynamicLine[j] = szInputBuffer[j];
    }
    //Insert a null terminator on new string
    szDynamicLine[j] = '\0';
    //free the input buffer
    free(szInputBuffer);

    return(szDynamicLine);   // if there is any error!
}
