/*************************************
* Lab 3 Exercise 4
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from 
// ex4_driver.c in your code.
extern int num_of_cars;
extern int num_of_segments;
sem_t *segment_sems;
sem_t cars_in_roundabout;

void initialise()
{
    segment_sems = malloc(sizeof(sem_t)*num_of_segments);
    for (int i=0; i < num_of_segments; i++) {
        sem_init(&segment_sems[i], 0, 1);
    }
    sem_init(&cars_in_roundabout, 0, num_of_segments-1); // general sem

}
void cleanup()
{
    for (int i=0; i < num_of_segments; i++) {
        sem_destroy(&segment_sems[i]);
    }
    free(segment_sems);
    sem_destroy(&cars_in_roundabout);
}

void* car(void* car)
{
    //A car: 
    //   1. should call enter_roundabout (...)
    //   2. followed by some calls to move_to_next_segment (...)
    //   3. finally call exit_roundabout (...)
    car_struct* current_car = (car_struct*) car;
    sem_wait(&cars_in_roundabout); // count cars in roundabout
    sem_wait(&segment_sems[current_car->entry_seg]);
    enter_roundabout(car);
    while(current_car->current_seg != current_car->exit_seg) {
        sem_wait(&segment_sems[NEXT(current_car->current_seg, num_of_segments)]); // wait for next
        move_to_next_segment(current_car);
        sem_post(&segment_sems[PREV(current_car->current_seg, num_of_segments)]); // exit prev
    }
    exit_roundabout(current_car);
    sem_post(&cars_in_roundabout);
    sem_post(&segment_sems[current_car->exit_seg]);
    pthread_exit(NULL);
}
