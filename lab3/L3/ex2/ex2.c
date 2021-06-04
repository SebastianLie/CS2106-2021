/*************************************
* Lab 3 Exercise 2
* Name:
* Student Id: A164657U
* Lab Group: B18
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <pthread.h>
#include <semaphore.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_mutex_init(&(lock->roomEmpty), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->roomEmpty));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->roomEmpty));
}

void reader_acquire(rw_lock* lock)
{
  //special change to lect soln:
  //bc of the way driver code is written
  //we need to inc only AFTER room is empty
  pthread_mutex_lock(&(lock->mutex));
  if (lock->reader_count == 0) {
    pthread_mutex_lock(&(lock->roomEmpty));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex)); 
}

void reader_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  lock->reader_count--;
  if (lock->reader_count == 0) {
    pthread_mutex_unlock(&(lock->roomEmpty));
  }
  pthread_mutex_unlock(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->roomEmpty));
  pthread_mutex_destroy(&(lock->mutex));
}