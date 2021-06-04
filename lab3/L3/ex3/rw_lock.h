/*************************************
* Lab 3 Exercise 3
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/************************************

You should use ex2 solution as the starting point.

Copy over the solution and modify as needed.
************************************/


#ifndef __CS2106_RWLOCKV2_H_
#define __CS2106_RWLOCKV2_H_

#include <pthread.h> // To import the pthread_mutex_t type.

typedef struct {
  pthread_mutex_t mutex1;
  pthread_mutex_t mutex2;
  pthread_mutex_t noReaders;
  pthread_mutex_t noWriters;
  int reader_count;
  int writer_count;
} rw_lock;

//Note: You are NOT allowed to change the function headers
void initialise(rw_lock* lock);
void writer_acquire(rw_lock* lock);
void writer_release(rw_lock* lock);
void reader_acquire(rw_lock* lock);
void reader_release(rw_lock* lock);
void cleanup(rw_lock* lock);

#endif
