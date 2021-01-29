/*
 * File: driver1.c
 * YOUR NAME ... YOU NEED TO IMPLEMENT THE main() + additional functions if needed
 *
 * ....
 */
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include "myio.h"
#include <limits.h>
#include <string.h>

int checkIfInt(char *num);
 /*
  * First one (say driver1.c) gets three command-line arguments: x y z.
  * It then asks user to enter x many integers, y many doubles, and z many lines.
  * Prompt for each input at a time.
  *
  * Everytime the program prints back the entered data on the stdout
  *      while printing error messages on stderr (if any).
  *
  * It also keeps track of the largest integer, largest double, and the longest string.
  * At the end, your program (driver1.c) prints the largest integer, the largest double,
  * and the longest line (longest string that were entered.)
  */

int main(int argc, char* argv[])
{
    // YOU NEED TO IMPLEMENT THIS (USE FUNCTIONS FROM myio.h) 
    // IF NEEDED, YOU CAN ALSO IMLEMENT YOUR OWN FUNCTIONS HERE
    int x, y, z, i, anInt;
    int maxInt = INT_MIN;
    double aDouble;
    double maxDouble = LONG_MIN;
    char aLine[1024] = { '\0' };
    char* longLine[1024] = { '\0' };

    //Checking that the correct number of parameters are entered into the command
    if (argc != 4) {
        printf("Invalid input parameters. Usage: (program) (int x) (int y) (int z)");
        exit(1);
    }

    //Verifies that all of the inputs are integers
    for (i = 1; i <= 3; i++) {
        if (checkIfInt(argv[i]) != 0) {
            printf("Command argument %d is not an integer, enter integers for input", i);
            exit(1);
        }
    }

    //Sets x, y, and z to the number of times ReadInteger, ReadDouble, and ReadLine will be looped.
    x = atoi(argv[1]);
    y = atoi(argv[2]);
    z = atoi(argv[3]);

    //Loop through ReadInteger x number of times prompting the user to enter an integer to stdin
    for (i = 0; i < x; i++) {
        anInt = ReadInteger();
        //prints that integer to the screen
        printf("The integer you entered is: %d\n", anInt);
        //Keeps track of the largest integer entered
        if (anInt > maxInt) {
            maxInt = anInt;
        }
    }

    //Loop through ReadDouble y number of times prompting the user to enter an double to stdin
    for (i = 0; i < y; i++) {
        aDouble = ReadDouble();
        //Prints the double that was entered to the screen
        printf("The double you entered: %f\n", aDouble);
        //Keeps track of the largest double entered by the user
        if (aDouble > maxDouble) {
            maxDouble = aDouble;
        }
    }
    
    //Loop through ReadLine z number of times prompting the user to enter an line to stdin
    for (i = 0; i < z; i++) {
        printf("Enter a line: ");
        strcpy(aLine, ReadLine());
        //Prints the string to the screen
        printf("The line you just entered is: %s\n", aLine);
        //Keeps track of the longest line the user entered
        if (strlen(aLine) > strlen(longLine)) {
            strcpy(longLine, aLine);
        }
    }
    
    //Prints out the largest int and double as well as the longest line entered.
    printf("The largest integer entered is: %d\n", maxInt);
    printf("The largest double entered is: %f\n", maxDouble);
    printf("The longest line entered is: %s\n", longLine);

    return 0;
}

int checkIfInt(char *num)
{
    int i;

    //Loops through to check if command argument is in the range of integers
    for (i = 0; num[i] != '\0' ; i++) {
        if (num[i] > '9' || num[i] < '0') {
            return -1;
        }
    }
    return 0;
}