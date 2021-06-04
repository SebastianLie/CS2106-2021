#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* example( void* arg )
{    
    int parameter;
    int* resultPtr;

    parameter = *((int*) arg);

    resultPtr = (int*)malloc(sizeof(int));
    *resultPtr = parameter+1000;

    printf("Thread: Parameter = %d, Return = %d\n",
        parameter, *resultPtr);

    return (void*)resultPtr;
}

int main()
{
    pthread_t tid;
    int input, *resultPtr;

    printf("input:");
    scanf("%d", &input);

    pthread_create( &tid, NULL, example, (void*)&input);    
   
    pthread_join( tid, (void*) &resultPtr );
        
    printf("Main: Thread %d returns %d\n", tid, *resultPtr);  

    free( resultPtr);
    resultPtr = NULL;  //Good practice

    return 0;
}
