#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include <fcntl.h>
#include<sys/stat.h>
#include <unistd.h>
#include <signal.h>
#define MAX 100

struct mymsg{
      long int mtype;
      char mtext[MAX];
};


void *sender1(){
       int n;
       struct mymsg message;
       char buf[MAX];
       sem_t *mutex = sem_open("mutex", O_CREAT | O_RDWR, 0666, 0);
       key_t key = ftok("/tmp",66);
       int msqid = msgget(key, 0666 | IPC_CREAT);
       if( msqid == -1){
             printf("create failed");
             exit(-1);
       }

       while(1){
             sem_wait(mutex);
             printf("sender1 input :");
             scanf("%s", buf);
             printf("\n");
             message.mtype = 1;
             if(strcmp(buf,"exit") == 0){
                     strcpy(message.mtext,"end1");
                     n = msgsnd(msqid, (void *)&message, 100, 0);
                    
                     n = msgrcv(msqid, (void *)&message, 100, 2, 0);
                     printf("---sender1 receive message from receive,the content is %s----\n", message.mtext); 
                     sem_post(mutex);
                     sleep(1);
                     return 0;
             }
             else{
                     strcpy(message.mtext,buf);
                     n = msgsnd(msqid, (void *)&message, 100, 0);
                     sem_post(mutex);
                     sleep(1);
             }

       }

}


void *sender2(){
       int n;
       struct mymsg message;
       char buf[MAX];
       sem_t *mutex = sem_open("mutex", O_CREAT | O_RDWR, 0666, 0);
       key_t key = ftok("/tmp",66);
       int msqid = msgget(key, 0666 | IPC_CREAT);
       if( msqid == -1){
             printf("create failed");
             exit(-1);
       }

       while(1){
             sem_wait(mutex);
             printf("sender2 input :");
             scanf("%s", buf);
             printf("\n");
             message.mtype = 1;
             if(strcmp(buf,"exit") == 0){
                     strcpy(message.mtext,"end2");
                     n = msgsnd(msqid, (void *)&message, 100, 0);
                     n = msgrcv(msqid, (void *)&message, 100, 3, 0);
                     printf("---sender2 receive message from receive,the content is %s--\n", message.mtext);
                     
                     sem_post(mutex);
                     sleep(1);
                     return 0;
             }
             else{
                     strcpy(message.mtext,buf);
                     n = msgsnd(msqid, (void *)&message, 100, 0);
                     sem_post(mutex);
                   sleep(1);
             }
       }

}


void *receive(){
           int n;
           int flag1=0;
           int flag2=0;
           struct mymsg message;
           char buf[MAX];
           key_t key = ftok("/tmp",66);
           int msqid = msgget(key, 0666 | IPC_CREAT);

           if( msqid == -1){
               printf("create failed");
               exit(-1);
           }
            sleep(5);
           while(1){
                   n = msgrcv(msqid, (void *)&message, 100, 1, 0);
                   if(n > 0){
                         printf("\n----receive message,the content is : %s----\n", message.mtext);
                         if( strcmp(message.mtext,"end1") == 0 ){
                               message.mtype = 2;
                               strcpy(message.mtext,"over1");
                               n = msgsnd(msqid, (void *)&message, 100, 0);
 
                               flag1 = 1;
                         }else if( strcmp(message.mtext,"end2") == 0 ){
                               message.mtype = 3;
                               strcpy(message.mtext,"over2");
                               n = msgsnd(msqid, (void *)&message, 100, 0);
                            
                               flag2 = 1;
                         }

                   }
                   if(flag1==1 && flag2==1){
                       
                       return 0;
                   }
                   sleep(1);
           }

}





int main(){
           key_t key = ftok("/tmp",66);
           int msqid = msgget(key, 0666 | IPC_CREAT);
           msgctl(msqid, IPC_RMID, 0);
           sem_unlink("mutex");

           sem_t *mutex = sem_open("mutex", O_CREAT | O_RDWR, 0666, 0);

           pthread_t pt1,pt2,pt3;
           pthread_create(&pt1, NULL, sender1, NULL);
           pthread_create(&pt2, NULL, sender2, NULL);
           pthread_create(&pt3, NULL, receive, NULL);
           sem_post(mutex);
           pthread_join(pt1, NULL);
           pthread_join(pt2, NULL);
           pthread_join(pt3, NULL);
           msgctl(msqid, IPC_RMID, 0);
           return 0;
}
