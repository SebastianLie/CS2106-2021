#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int userInput, childPid, childResult;
    //Since largest number is 10 digits, a 12 characters string is more
    //than enough
    char cStringExample[12];

    scanf("%d", &userInput);
    int nums[userInput];
    for (int i=0; i<=userInput;) {
        scanf("%d", nums[i]);
    }
    for (int i=0; i<=userInput;) {
        childPid = fork();
        if (childPid != 0 ){
            wait( &childResult);
            printf("%d has %d prime factors\n", userInput, childResult >> 8);

        } else {
            //Easy way to convert a number into a string
            sprintf(cStringExample, "%d", userInput);

            execl("./PF", "PF", cStringExample, NULL);
        }
    }
    
    

    
      
}
