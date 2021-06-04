#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int PrimeFactorization( int userInput )
{    
    int nFactor, factor;
   
    nFactor = 0;
    factor = 2;

    //quick hack to get the number of prime factors
    // only for positive integer
    while (userInput > 1){
        if (userInput % factor == 0){
            userInput /= factor;
            nFactor++;
        } else {
            factor++;
        }
    }

    return nFactor;
}


int main()
{
    int i, userInput[9], nInput;

    scanf("%d", &nInput);

    //Crate thread for each user input
    for (i = 0; i < nInput; i++){
        scanf("%d", &userInput[i]);

        //create thread
    }
   

    //Wait for each thread in order
    for (i = 0; i < nInput; i++){
       
    }

    return 0;
}
