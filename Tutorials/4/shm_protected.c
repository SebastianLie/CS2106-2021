#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
    int i, shmid, n = 100, childPid, nChild = 1, pOrder = 0;
    int *shm;

    if (argc > 1)
        n = atoi(argv[1]);

    if (argc > 2)
        nChild = atoi(argv[2]);

    // create Share Memory Region
    // read, write, excute
    // 1      1      0        = 6
    // 1      0      0        = 4
    
    shmid = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0600);
    if (shmid == -1) {
        printf("Cannot create shared memory!\n");
        exit(1);
    }
    else {
        printf("Shared Memory Id = %d\n", shmid);
    }

    // TODO: attach the shared memory region to this process
    // Attach the shared memory region to this process
    shm = (int*)shmat(shmid, NULL, 0);
    if (shm == (int*) -1) {
        printf("Cannot attach shared memory!\n");
        exit(1);
    }
    shm[0] = 0;    // initialize shared memory to 0
    shm[1] = 0;

    for (i = 0; i < nChild; i++) {
        childPid = fork();
        if (childPid == 0) {
            pOrder = i + 1; // each process gets its pOrder
            break;
        }
    }
    while (shm[1] != pOrder); 
    for (i = 0; i < n; i++)
        shm[0]++;

    if (shm[1] == pOrder) {
        shm[1]++;
    }

    if (childPid != 0) {
        for (i = 0; i < nChild; i++)
            wait(NULL);
        printf("The value in the shared memory is: %d\n", shm[0]);
        // detach and destroy
        shmdt((char*)shm);
        shmctl( shmid, IPC_RMID, 0);
    }

    return 0;
}
