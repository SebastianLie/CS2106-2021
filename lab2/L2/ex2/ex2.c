/*************************************
* Lab 2 Exercise 2
* Name: Sebastian Lie
* Student Id: A0164657U
* Lab Group: B15
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
Note2: Exercise 3 is a superset of exercise 2
Please copy over your code to ex3.c and continue
Submit ONLY ex3.c (even if you cannot finish ex3)
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

// works in compute cluster!
char** split( char* input, char* delimiter, int maxTokenNum, int* readTokenNum )
//Assumptions:
//  - the input line is a string (i.e. with NULL character at the end)
//  - the delimiter is a string of possible delimiters, each delimiter is single chracter
//Behavior:
//  - Split up to and include maxTokenNum from the input string
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - Must use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* tStart;   //start of token
    int i, strSize;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum );

    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++){
        tokenStrArr[i] = NULL;
    }

    //Look for next token as separated by the delimiter
    tStart = strtok(input, delimiter); 

    i = 0;
    while (i < maxTokenNum && tStart){

        strSize = strlen(tStart);

        //Allocate space for token string. +1 for null terminator
        tokenStrArr[i] = (char*) malloc(sizeof(char) * (strSize + 1) );

        strcpy(tokenStrArr[i], tStart);    

        i++;
        tStart = strtok(NULL, delimiter); 
    }

    //return number of token read
    *readTokenNum = i;

    return tokenStrArr;
}

void freeTokenArray(char** strArr, int size)
{
    int i;

    //Free each string first
    for (i = 0; i < size; i++){
        if (strArr[i])
            free( strArr[i] );
        strArr[i] = NULL;
    }

    //Free the entire array
    free( strArr );

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}

void command(char* path, char** args)
{
    struct stat fileStat;
    char* tmp = strcat(path, "/");
    char* commandPath = strcat(tmp, args[0]);
    if (stat(commandPath, &fileStat) < 0) {
        printf("\"%s\" not found \n", commandPath);
        return;
    }
    int childPid = fork();
    if (childPid == 0) {
        // u r the child
        // make new path with command first
        execv(commandPath, args);
        return;
    } else {
        // and I m the parent
        wait(NULL);
        return;
    }
    // yeah duh bitch
}


int main()
{
    char **cmdLineArgs;
    char path[20] = ".";  //default search path
    char userInput[121];

    int tokenNum;

    //read user input
    printf("YWIMC > ");

    //read up to 120 characters from user input
    fgets(userInput, 120, stdin);

    //split is a helper function to "tokenize" the input
    //see "stringTokenizer.c" program for more sample usage 
    cmdLineArgs = split( userInput, " \n", 7, &tokenNum );

    //At this point you have the user input split neatly into token in cmdLineArg[]

    while ( strcmp( cmdLineArgs[0], "quit") != 0 ){
        //Figure out which command the user want and implement below
        if (strcmp(cmdLineArgs[0], "showpath") == 0) {
            printf("%s\n", path);
        }
        else if (strcmp( cmdLineArgs[0], "setpath") == 0) {
            strcpy(path, cmdLineArgs[1]);
        }
        else {
            char newPath[20];
            strcpy(newPath, path);
            command(newPath, cmdLineArgs);
        }
        //Prepare for next round input

        //Clean up the token array as it is dynamically allocated
        freeTokenArray(cmdLineArgs, tokenNum);

        printf("YWIMC > ");
        fgets(userInput, 120, stdin);
        cmdLineArgs = split( userInput, " \n", 7, &tokenNum );
    }
    

    printf("Goodbye!\n");

    //Clean up the token array as it is dynamically allocated
    freeTokenArray(cmdLineArgs, tokenNum);


    return 0;

}
