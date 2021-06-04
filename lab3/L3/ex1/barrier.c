/*************************************
* Lab 3 Exercise 1
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/**
 * CS2106 AY 20/21 Semester 2 - Lab 3
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "barrier.h"


// Initialise barrier here
void barrier_init ( barrier_t *barrier, int count ) 
{
    barrier->count = count;
    barrier->mutex = malloc(sizeof(sem_t));
    sem_init(barrier->mutex, 0, 0);
}

void barrier_wait ( barrier_t *barrier ) 
{
    barrier->count--;
    if (barrier->count == 0) {
        sem_post(barrier->mutex);
    }
    sem_wait(barrier->mutex);
    sem_post(barrier->mutex);
}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) 
{
    sem_destroy( barrier->mutex );
    free( barrier->mutex );
}
