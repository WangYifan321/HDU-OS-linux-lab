#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>
#include <wait.h>

int main()
{
    
    sem_t *Mutex; 
    sem_t *send1, *send2, *send3;
    sem_t *receive1, *receive2, *receive3;
    pid_t pid1, pid2, pid3;
    char readbuf[1024];
    int readbuflen;
    int buflen = 0;
    int fd[2], flag; //fd[0]-->read,fd[1]-->write
    char buf[1024];
    printf("blocked read,please input how much bytes to read:");
    scanf("%d", &readbuflen);
    getchar();
    memset(buf, 0, sizeof(buf));
    //delete before fuzhi
    sem_unlink("Mutex");
    sem_unlink("send1");
    sem_unlink("send2");
    sem_unlink("send3");
    sem_unlink("receive1");
    sem_unlink("receive2");
    sem_unlink("receive3");
    //0666-->read and write
    Mutex = sem_open("Mutex", O_CREAT, 0666, 1);
    send1 = sem_open("send1", O_CREAT, 0666, 1);
    send2 = sem_open("send2", O_CREAT, 0666, 1);
    send3 = sem_open("send3", O_CREAT, 0666, 1);
    receive1 = sem_open("receive1", O_CREAT, 0666, 0);
    receive2 = sem_open("receive2", O_CREAT, 0666, 0);
    receive3 = sem_open("receive3", O_CREAT, 0666, 0);
    flag = pipe(fd); //no named pipe
    if (flag == -1)  //create defeat
    {
        printf("pipe create error\n");
        return 1;
    }
    pid1 = fork();
    if (pid1 > 0)
    {
        pid2 = fork();
        if (pid2 > 0)
        {
            pid3 = fork();
        }
    }
    if (pid1 < 0 || pid2 < 0 || pid3 < 0)
    {
        sem_unlink(Mutex);
        sem_unlink(send1);
        sem_unlink(send2);
        sem_unlink(send3);
        sem_unlink(receive1);
        sem_unlink(receive2);
        sem_unlink(receive3);
        printf("fork error\n");
        return 2;
    }
    sleep(1);
    if (pid1 == 0)
    {
        while (1)
        {
        close(fd[0]);
        sem_wait(send1);
        sem_wait(Mutex);
        printf("pid:%d pthread 1 input data:",getpid());
        scanf("%[^\n]%*c",buf);
        write(fd[1],buf,strlen(buf));
        buflen+=strlen(buf);
        printf("total read %d bytes,present son pthread give %d bytes\n",readbuflen,strlen(buf));   
        sleep(1);
        sem_post(Mutex);
        sem_post(receive1);
        sleep(5);
        }
    }
    else if (pid2 == 0)
    {
        while (1)
        {
        close(fd[0]);
        sem_wait(send2);
        sem_wait(Mutex);
        printf("pid:%d pthread 2 input data:",getpid());
        scanf("%[^\n]%*c",buf);
        write(fd[1],buf,strlen(buf));
        buflen+=strlen(buf);
        printf("total read %d bytes,present son pthread give %d bytes\n",readbuflen,strlen(buf));   
        sleep(1);
        sem_post(Mutex);
       sem_post(receive2);
        sleep(5);
        }
        
    }
    else if (pid3 == 0)
    {
        while (1)
        {
        close(fd[0]);
        sem_wait(send3);
        sem_wait(Mutex);
        printf("pid:%d pthread 3 input data:",getpid());
        scanf("%[^\n]%*c",buf);
        write(fd[1],buf,strlen(buf));
        buflen+=strlen(buf);
        printf("total read %d bytes,present son pthread give %d bytes\n",readbuflen,strlen(buf));   
        sleep(1);
        sem_post(Mutex);
        sem_post(receive3);
        sleep(5);
        }
        
    }
    else
    { 
        while (1)
    {      
        printf("father pthread is reading:\n");
        close(fd[1]);
        sem_wait(receive1);
        sem_wait(receive2);
        sem_wait(receive3);
        sem_wait(Mutex);
        read(fd[0], buf, 1024);
        strcat(readbuf,buf);
        if (strlen(readbuf)<readbuflen)
        {   
            printf("read waiting,,the pipe's data is not enough,read blocked.\n");
            sem_post(Mutex);
            sem_post(send1);
            sem_post(send2);
            sem_post(send3);
        }
        else
        {
             
             
             printf("pid:%d parents pthread get data:",getpid());
             for(int i=0;i<=readbuflen;i++)
             printf("%c",readbuf[i]);
             printf("\n");
            sem_post(Mutex);
            sem_post(send1);
            sem_post(send2);
            sem_post(send3);
            break;
        }
    }    
 }  
}
