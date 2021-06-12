#include "common.h"

pthread_t r_thread, s_thread;

void *send(void *arg)
{
    char temp[100], s_str[100];
    while(1)
    {
	printf("receiver send message:");
	fflush(stdout);
	scanf("%[^\n]%*c",s_str);
        sem_wait(sem_sender);
        sprintf(temp,"%d:%s",*((pid_t *)arg),s_str);
        strcpy((char *)shmp, temp);
        
        sem_post(sem_receiver1);
	printf("OK\n");
        if(!strcmp(s_str,"over"))
        {
            pthread_cancel(r_thread);
            shmdt(shmp);
	    break;
        }
    }
}

void *receive(void *arg)
{

    char r_str[100], *p;
    while(1)
    {
        sem_wait(sem_receiver2);
        strcpy(r_str, (char *)shmp);
        p = strchr(r_str,':');
        *(p++) = '\0';
        printf("\rget message from %s:%s\n", r_str,p);
        if(strcmp(p, "over") == 0)
        {
            pthread_cancel(s_thread);
            shmdt(shmp);
            shmctl(shmid, IPC_RMID, NULL);
            sem_unlink("sender");
            sem_unlink("receiver1");
            sem_unlink("receiver2");
            break;
        }
	printf("receiver send message:");
	fflush(stdout);
        sem_post(sem_sender);
    }
}

int main()
{
    pid_t pid = getpid();
    int p1 = 0,p2 = 0;

    init_signal();
    
    printf("pthread %d start!\n",pid);
    p1 = pthread_create(&s_thread,NULL,send,&pid);
    p2 = pthread_create(&r_thread,NULL,receive,&pid);
    if(p1 || p2)
    {
        printf("create pthread failed\n");
        return 1;
    }
    pthread_join(s_thread, NULL);
    pthread_join(r_thread, NULL);
    printf("pthread %d game over!\n",pid);
}
