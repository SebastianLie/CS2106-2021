/*************************************
* Lab 4 Exercise 4
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


/**********************************************************
 * Quality of life helper functions / macros
 *********************************************************/
#define powOf2(E) (1 << E)

unsigned int log2Ceiling( unsigned int N )
/**********************************************************
 * Find the smallest S, such that 2^S >= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 < N){
        pOf2 <<= 1;
        s++;
    }

    return s;
}


unsigned int log2Floor( unsigned int N )
/**********************************************************
 * Find the largest S, such that 2^S <= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 <= N){
        pOf2 <<= 1;
        s++;
    }

    return s-1;
}

unsigned int buddyOf( unsigned int addr, unsigned int lvl )
/**********************************************************
 * Return the buddy address of address (addr) at level (lvl)
 *********************************************************/
{
    unsigned int mask = 0xFFFFFFFF << lvl;
    unsigned int buddyBit = 0x0001 << lvl;

    return (addr & mask) ^ buddyBit;
}

partInfo* buildPartitionInfo(unsigned int offset)
/**********************************************************
 * Allocate a new partInfo structure and initialize the fields
 *********************************************************/
{
    partInfo *piPtr;

    piPtr = (partInfo*) malloc(sizeof(partInfo));

    piPtr->offset = offset;
	piPtr->nextPart = NULL;

    //Buddy system's partition size is implicit
	//piPtr->size = size;

    //All available partition in buddy system is implicitly free
	//piPtr->status = FREE;

    return piPtr;
}

void printPartitionList(partInfo* piPtr)
/**********************************************************
 * Print a partition linked list
 *********************************************************/
{
	partInfo* current;
    int count = 1;
	
	for ( current = piPtr; current != NULL; 
		current = current->nextPart){
        if (count % 8 == 0){
            printf("\t");
        }
		printf("[+%5d] ", current->offset);
        count++;
        if (count % 8 == 0){
            printf("\n");
        }
	}
    printf("\n");
}

void printHeapMetaInfo()
/**********************************************************
 * Print Heap Internal Bookkeeping Information
 *********************************************************/
{
    int i;

	printf("\nHeap Meta Info:\n");
	printf("===============\n");
	printf("Total Size = %d bytes\n", hmi.totalSize);
	printf("Start Address = %p\n", hmi.base);

    for (i = hmi.maxIdx; i >=0; i--){
        printf("A[%d]: ", i);
        printPartitionList(hmi.A[i] );
    }

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
    //TODO: Task 4. Calculate and report the various statistics

    printf("\nHeap Usage Statistics:\n");
    printf("======================\n");

    printf("Total Space: %d bytes\n", hmi.totalSize);

   //Remember to preserve the message format!
   int freeParts = 0;
   int freeSize = 0;
   for (int i=0;i<=hmi.maxIdx;i++) {
       partInfo* currPart = hmi.A[i];
       if (currPart == NULL) {
           // no partitions here, move on
           continue;
       }
       // otherwise count parts
       while (currPart != NULL) {
           freeParts += 1;
           freeSize += powOf2(i);
           currPart = currPart->nextPart;
       }
   }
    printf("Total Free Partitions: %d\n", freeParts);
    printf("Total Free Size: %d bytes\n", freeSize);

    printf("Total Internal Fragmentation: %d bytes\n", hmi.internalFragTotal);
    }

void addPartitionAtLevel( unsigned int lvl, unsigned int offset )
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function adds a new free partition with "offset" at hmi.A[lvl]
 *    If buddy is found, recursively (or repeatedly) perform merging and insert
 *      at higher level
 *********************************************************/
{
    unsigned int baseAddr = (char*)hmi.base - (char*)0;
    unsigned int freedPartAddr = baseAddr + offset;
    unsigned int buddyPartAddr = buddyOf(freedPartAddr, lvl);
    unsigned int buddyPartOffset = buddyPartAddr - baseAddr;
    partInfo* currPart = hmi.A[lvl];
    partInfo* beforePart = currPart;
    while (currPart != NULL) {
        if (currPart->offset == buddyPartOffset) {
            break;
        }
        beforePart = currPart;
        currPart = currPart->nextPart;
    }
    if (currPart != NULL) {
        // buddy found
        // newOffset = offset if offset less than buddy offset
        unsigned int newOffset = offset < buddyPartOffset ? offset : buddyPartOffset;
        // check if head of list
        if (currPart == hmi.A[lvl]) {
            // update head
            hmi.A[lvl] = currPart->nextPart;
        } else {
            // otherwise deal with removing from linked list
            beforePart->nextPart = currPart->nextPart;
            currPart->nextPart = NULL;
        }
        free(currPart);
        // add partition at higher level
        addPartitionAtLevel(lvl+1, newOffset);
    } else {
        // no buddy
        partInfo* newPart = buildPartitionInfo(offset);
        // check if already have partition at level
        partInfo* oldPart = hmi.A[lvl];
        if (oldPart != NULL) {
            // have partition, we have to search 
            // where to plant new partition
            partInfo* nextPart = hmi.A[lvl]->nextPart;
            if (offset < oldPart->offset) {
                // new part at head of list
                hmi.A[lvl] = newPart;
                newPart->nextPart = oldPart;
            } else {
                // new is not head, we go a-huntin'
                while (nextPart != NULL) {
                    // keep exploring parts until 
                    // either part that is after newPart is found
                    // or we get to end of linked list
                    if (offset < nextPart->offset) {
                        break;
                    }
                    oldPart = nextPart;
                    nextPart = oldPart->nextPart;

                }
                oldPart->nextPart = newPart;
                newPart->nextPart = nextPart;
            }
        } else {
            hmi.A[lvl] = newPart;
        }
    }

}

partInfo* removePartitionAtLevel(unsigned int lvl)
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function remove a free partition at hmi.A[lvl]
 *    Perform the "upstream" search if this lvl is empty AND perform
 *      the repeated split from higher level back to this level.
 * 
 * Return NULL if cannot find such partition (i.e. cannot satisfy request)
 * Return the Partition Structure if found.
 *********************************************************/
{
    // check if lvl is not past max
    if (lvl <= hmi.maxIdx) {
        // check if partition present
        if (hmi.A[lvl] == NULL) { 
            // no partition, go upstream to create one
            // not at max, can still search upstream
            partInfo* splitted = removePartitionAtLevel(lvl + 1);
            if (splitted != NULL) {
                // upper level partition found at lvl + 1, 
                // so we create a new partition at new offset
                // and return the first
                int newOffset = powOf2(lvl) + splitted->offset;
                addPartitionAtLevel(lvl, newOffset);
                return splitted;
            }

        } else {
            // there is partition means there is free space!
            // save partition found, then remove from array
            partInfo* newPart = hmi.A[lvl];
            hmi.A[lvl] = hmi.A[lvl]->nextPart;
            return newPart;
        }

    }
    return NULL;
}

int setupHeap(int initialSize, int minSize, int maxSize)
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

    hmi.base = base;
    hmi.minIdx = log2Floor(minSize);
    hmi.maxIdx = log2Floor(maxSize);
    hmi.internalFragTotal = 0;
    // figure out number of levels for A[]
    int levels = log2Floor(initialSize);

    // dynamically allocate A[]
    partInfo** A;
    A = malloc((levels + 1) * sizeof(partInfo*));
    int offset = 0;
    for (int i=0; i<=levels; i++) {
        A[i] = NULL;
    }
    while (initialSize > minSize) {
        int largestPowOf2 = log2Floor(initialSize);
        // handle max/min size
        while (largestPowOf2 > hmi.maxIdx) {
            largestPowOf2--;
        }
        while (largestPowOf2 < hmi.minIdx) {
            largestPowOf2++;
        }
        partInfo* currPart = buildPartitionInfo(offset);
        offset += powOf2(largestPowOf2);
        partInfo* endOfList = A[largestPowOf2];
        if (endOfList == NULL) {
            A[largestPowOf2] = currPart;
        } else {
            // A[largestPowOf2] alr have partition
            partInfo* beforePart = endOfList;
            while (endOfList != NULL) {
                // find end of linked list to put new part to
                // save latest partition into beforePart
                beforePart = endOfList;
                endOfList = endOfList->nextPart;
            }
            beforePart->nextPart = currPart;
        }
        initialSize -= powOf2(largestPowOf2);
    }
    //TODO: Task 1. Setup the rest of the bookkeeping info:
    //       hmi.A <= an array of partition linked list
    //       hmi.maxIdx <= the largest index for hmi.A[]
    hmi.totalSize = offset;
    hmi.A = A;
    hmi.maxIdx = levels;
    return 1;
}


void* mymalloc(int size)
/**********************************************************
 * Mimic the normal "malloc()":
 *    Attempt to allocate a piece of free heap of (size) bytes
 *    Return the memory address of this free memory if successful
 *    Return NULL otherwise 
 *********************************************************/
{
    //TODO: Task 2. Implement the allocation using buddy allocator

    // 1. Find the smallest S, such that 2^S >= size
    unsigned int S = log2Ceiling(size);
    // adjust S if smaller than min or if bigger than max.
    S = S < hmi.minIdx ? hmi.minIdx : S;
    S = S > hmi.maxIdx ? hmi.maxIdx : S;
    // 2. Access hmi.A[S] for a free block
    partInfo* fittingPart = removePartitionAtLevel(S);
    if (fittingPart == NULL) {
        return NULL;
    }
    hmi.internalFragTotal = hmi.internalFragTotal + (powOf2(S) - size);
    return (void*)hmi.base + fittingPart->offset;

}

void myfree(void* address, int size)
/**********************************************************
 * Mimic the normal "free()":
 *    Attempt to free a previously allocated memory space
 *********************************************************/
{
    //TODO: Task 3. Implement the de allocation using buddy allocator
    // 1. Find the smallest S, such that 2^S >= size
    unsigned int S = log2Ceiling(size);
    // adjust S if smaller than min or if bigger than max.
    S = S < hmi.minIdx ? hmi.minIdx : S;
    S = S > hmi.maxIdx ? hmi.maxIdx : S;
    // 2. Free the block needed
    int offset = (char*)address - (char*)hmi.base;
    addPartitionAtLevel(S, offset);
    hmi.internalFragTotal = hmi.internalFragTotal - (powOf2(S) - size);
}
