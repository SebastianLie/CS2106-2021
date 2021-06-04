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
Solution here from Little book of Semaphores, 
followed as closely as possible for our driver code.
************************************/

#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  // follow soln for writer priority in Little Book of Semaphores, but 
  // tweak to driver code.
  pthread_mutex_init(&(lock->noReaders), NULL);
  pthread_mutex_init(&(lock->mutex1), NULL);
  pthread_mutex_init(&(lock->mutex2), NULL);
  pthread_mutex_init(&(lock->noWriters), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{

  if (lock->writer_count == 0) { 
    // want 1st writer to prevent further readers
    // from coming in.
    pthread_mutex_lock(&(lock->noReaders));
  }
  pthread_mutex_lock(&(lock->noWriters)); // 1 writer at a time
  pthread_mutex_lock(&(lock->mutex2));
  lock->writer_count++;  // protect shared var
  pthread_mutex_unlock(&(lock->mutex2));
}

void writer_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex2));
  lock->writer_count--; // protect shared var 
  pthread_mutex_unlock(&(lock->mutex2));
  pthread_mutex_unlock(&(lock->noWriters)); // next writer can enter
  if (lock->writer_count == 0) {
    // no writers than can read again
    pthread_mutex_unlock(&(lock->noReaders));
  }
}

void reader_acquire(rw_lock* lock)
{
  // only hold no readers within this fn
  pthread_mutex_lock(&(lock->noReaders));
  pthread_mutex_lock(&(lock->mutex1));
  if (lock->reader_count == 0) {
    pthread_mutex_lock(&(lock->noWriters));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex1));
  pthread_mutex_unlock(&(lock->noReaders)); 
}

void reader_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex1));
  lock->reader_count--;
  if (lock->reader_count == 0) {
    pthread_mutex_unlock(&(lock->noWriters));
  }
  pthread_mutex_unlock(&(lock->mutex1));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->mutex1));
  pthread_mutex_destroy(&(lock->mutex2));
  pthread_mutex_destroy(&(lock->noWriters));
  pthread_mutex_destroy(&(lock->noReaders));
}