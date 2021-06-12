//msar.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <pthread.h>
#include <semaphore.h>
#include <glob.h>

#define SIZE 1024

sem_t *sem_sender;
sem_t *sem_receiver1;
sem_t *sem_receiver2;

int shmid;
void *shmp;

void init_signal()
{
    sem_sender = sem_open("sender", O_CREAT, 0666, 1);
    sem_receiver1 = sem_open("receiver1", O_CREAT, 0666, 0);
    sem_receiver2 = sem_open("receiver2", O_CREAT, 0666, 0);
    key_t key = ftok("/tmp",666);

    shmid = shmget(key, SIZE, 0666|IPC_CREAT);
    if(shmid < 0)
    {
        printf("shmget error！\n");
        exit(-1);
    }

    shmp = shmat(shmid, NULL, 0);
    if((long long int)shmp == -1)
    {
        printf("shmp error！\n");
        exit(-1);
    }
}
