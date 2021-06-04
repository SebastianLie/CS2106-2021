/*************************************
* Lab 4 Exercise 2
* Name: Sebastian Lie
* Student Id: A0164657U
* Lab Group: B15
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmalloc.h"

/**********************************************************
 * This is a "global" structure storing heap information
 * The "static" modifier restrict the structure to be
 *  visible only to functions in this file
 *********************************************************/
static heapMetaInfo hmi;

partInfo* buildPartitionInfo(unsigned int offset, int size)
/**********************************************************
 * Allocate a new partInfo structure and initialize the fields
 *********************************************************/
{
    partInfo *piPtr;

    piPtr = (partInfo*) malloc(sizeof(partInfo));

    piPtr->offset = offset;
	piPtr->size = size;
	piPtr->nextPart = NULL;
	piPtr->status = FREE;

    return piPtr;
}

void printPartitionList(partInfo* piPtr)
/**********************************************************
 * Print a partition linked list
 *********************************************************/
{
	partInfo* current;
	
	for ( current = piPtr; current != NULL; 
		current = current->nextPart){

		printf("[+%5d | %5d bytes | %d]\n", 
				current->offset, current->size, current->status);
	}
}

void printHeapMetaInfo()
/**********************************************************
 * Print Heap Internal Bookkeeping Information
 *********************************************************/
{
	printf("\nHeap Meta Info:\n");
	printf("===============\n");
	printf("Total Size = %d bytes\n", hmi.totalSize);
	printf("Start Address = %p\n", hmi.base);
	printf("Partition list:\n");

	printPartitionList(hmi.pListHead);
	printf("\n");

}

void printHeap()
/**********************************************************
 * Print the content of the entire Heap 
 *********************************************************/
{
    //Included as last debugging mechanism.
    //Print the entire heap regions as integer values.

    int* array;
    int size, i;
    
    size = hmi.totalSize / sizeof(int);
    array = (int*)hmi.base;

    for ( i = 0; i < size; i++){
        if (i % 4 == 0){
            printf("[+%5d] |", i);
        }
        printf("%8d",array[i]);
        if ((i+1) % 4 == 0){
            printf("\n");
        }
    }
}


void printHeapStatistic()
/**********************************************************
 * Print Heap Usage Statistics
 *********************************************************/
{

    printf("\nHeap Usage Statistics:\n");
    printf("======================\n");

    partInfo* current = hmi.pListHead;

	int holeCount = 0;
	int holeTotalsize = 0;
	int occupiedSize = 0;
	int occupiedCount = 0;

	while(current != NULL) {
		if (current->status == FREE){ 
            //if its empty
            holeCount++;
            holeTotalsize = holeTotalsize + current->size;
		}
		else{
             //not empty
            occupiedCount++;
            occupiedSize = occupiedSize + current->size;
		}
		current = current->nextPart;
	}
	
    printf("Total Space: %d bytes\n", hmi.totalSize);
    printf("Total Occupied Partitions: %d\n", occupiedCount);
    printf("\tTotal Occupied Size: %d bytes\n", occupiedSize);
    printf("Total Number of Holes: %d\n", holeCount);
    printf("\tTotal Hole Size: %d bytes\n", holeTotalsize);

}

int setupHeap(int initialSize)
/**********************************************************
 * Setup a heap with "initialSize" bytes
 *********************************************************/
{
	void* base;

	base = sbrk(0);
	if(	sbrk(initialSize) == (void*)-1){
		printf("Cannot set break! Behavior undefined!\n");
		return 0;
	}

	hmi.totalSize = initialSize;
    hmi.base = base;
	
    //Setup the very first partition info structure
	hmi.pListHead = buildPartitionInfo( 0, initialSize );
	
	return 1;
}



void splitPart(partInfo *bigPart, int newSize)
/**********************************************************
 * Split a partition "bigPart" into two partitions:
 *    one with newSize bytes, 
 *    the other with (original_size - newSize) bytes
 *********************************************************/
{
	partInfo *holeAt;
	int holeSize;

	//Remember: When calculating address offset, use a pointer of
	//(void*) type (or char*), so that the calculation is in bytes.
	holeSize = bigPart->size - newSize;

	//Make a new partition for the hole
	holeAt = buildPartitionInfo(bigPart->offset+newSize, holeSize);

    //Linked list insertion
	holeAt->nextPart = bigPart->nextPart;
	bigPart->nextPart = holeAt;

    bigPart->size = newSize;

}

void* mymalloc(int size)
/**********************************************************
 * Mimic the normal "malloc()":
 *    Attempt to allocate a piece of free heap of (size) bytes
 *    Return the memory addres of this free memory if successful
 *    Return NULL otherwise 
 *********************************************************/
{
    //TODO: Modify the allocation algoritm from First-Fit to
    //       Best-Fit

	partInfo *current = hmi.pListHead;

    //We need to make sure the size is word
    // aligned, i.e. if the word size is 4 bytes, the size need to be
    // rounded to nearest multiples of 4. Otherwise, user can get "bus
    // error" when accessing non-aligned memory locations

    // Use simple arithmetic to achieve this purpose:
    //  - Divide by 4 then multiply by 4 gives rounded multiples of 4. 
    //  - Dddition of 4 round up to the next multiple 
    //  - subtraction take care of the case where size is already multiples of 4. 
    //This can be achieved via bitwise operation too.
    size = (size - 1) / 4 * 4 + 4;
    
    // Best-fit algo
    int minFreeSize = __INT_MAX__;
    partInfo* minPart = NULL;
    // find best fitting
    for (current = hmi.pListHead; current != NULL; current = current->nextPart) {
		if (current->size >= size && current->status == FREE) {
            if (current->size - size  < minFreeSize) { 
                // only update if size strictly smaller
                // ensure if same size, we choose part with smallest address
                minFreeSize = current->size - size;
                minPart = current;
            }
			
		}
	}	
    // set best fitting
	current = minPart;
    /*
    //First-fit algorithm
	while ( current != NULL && 
			(current->status == OCCUPIED || current->size < size) ){

		current = current->nextPart;
	}
    */
    if (current == NULL){	//heap full
		return NULL;
	}

	//Do we need to split the partition?
	if (current->size > size) {
		splitPart(current, size);
	}

	current->status = OCCUPIED;
	
	return (void*)hmi.base + current->offset;
}

void myfree(void* address)
/**********************************************************
 * Mimic the normal "free()":
 *    Attempt to free a previously allocated memory space
 *********************************************************/
{
	partInfo *toBeFreed;
    int partID;

    //Use the offset as a unique ID to look for the right partition
 	partID = address - hmi.base;
    
    for( toBeFreed = hmi.pListHead; 
        toBeFreed && toBeFreed->offset != partID;
        toBeFreed = toBeFreed->nextPart){

        //Essentially an empty for-loop at the moment
    
    }

    //Should not happen in this lab as we free only correct adddresses
    if (toBeFreed == NULL) {
        printf("MyFree(%p) failed! Exiting.\n", address);
        exit(1);
    }

    //Very simple handling, just set the partition to FREE
    toBeFreed->status = FREE;

    //If this is a full implementation, what should we do with freed partition?

}
