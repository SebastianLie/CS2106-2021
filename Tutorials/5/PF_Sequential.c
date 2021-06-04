#include <stdio.h>
#include <stdlib.h>

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
    int i, userInput, nInput, result;

    scanf("%d", &nInput);

    //Crate thread for each user input
    for (i = 0; i < nInput; i++){
        scanf("%d", &userInput);

        result = PrimeFactorization( userInput );

        printf("%d has %d prime factors\n", userInput, result);
    }

    return 0;
}
