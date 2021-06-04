#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    int i, j, userInput[9], nInput, childPid[9], childResult, pid;
    //Since largest number is 10 digits, a 12 characters string is more
    //than enough
    char cStringExample[12];

    scanf("%d", &nInput);

    for (i = 0; i < nInput; i++){
        scanf("%d", &userInput[i]);
    
        childPid[i] = fork();
        if (childPid[i] == 0){
            sprintf(cStringExample, "%d", userInput[i]);
            execl("./PF", "PF", cStringExample, NULL);

        }
    }
   

    for (i = 0; i < nInput; i++){
        pid = wait( &childResult );

        //match pid with child pid
        for (j = 0; j < nInput; j++){
            if (pid == childPid[j])
                break;
        }
        printf("%d has %d prime factors\n", userInput[j], childResult >> 8);  
    }
      
}
