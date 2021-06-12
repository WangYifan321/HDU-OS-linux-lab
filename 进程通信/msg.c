/*
1.ftok函数生成键值
每一个消息队列都有一个对应的键值（key）相关联（共享内存、信号量也同样需要）。
所需头文件#include<sys/ipc.h>
函数原型 key_t ftok(const char *path ,int id);

path为一个已存在的路径名
id为0~255之间的一个数值，代表项目ID，自己取
返回值：成功返回键值（相当于32位的int）。出错返回-1

例如：key_t key = ftok( “/tmp”, 66);
2.msgget函数创建消息队列
所需头文件#include<sys/msg.h>
函数原型 int msgget(key_t key,int flag);

key为ftok生成的键值
flag为所需要的操作和权限，可以用来控制创建一个消息队列。
flag的值为IPC_CREAT：如果不存在key值的消息队列，且权限不为0，则创建消息队列，并返回一个消息队列ID。如果存在，则直接返回消息队列ID。
flag的值为 IPC_CREAT | IPC_EXCL：如果不存在key值的消息队列，且权限不为0，则创建消息队列，并返回一个消息队列ID。如果存在，则产生错误。

返回值：成功返回消息队列ID；出错返回-1

例如：int id = msgget(key,IPC_CREAT|IPC_EXCL|0666);创建一个权限为0666（所有用户可读可写，具体查询linux权限相关内容）的消息队列，并返回一个整形消息队列ID，如果key值已经存在有消息队列了，则出错返回-1。

     int id = msgget(key,IPC_CREAT|0666);创建一个权限为0666（所有用户可读可写，具体查询linux权限相关内容）的消息队列，并返回一个消息队列ID，如果key值已经存在有消息队列了，则直接返回一个消息队列ID。

3.msgsnd函数往消息队列发送消息
所需头文件#include<sys/msg.h>
函数原型 int msgsnd(int msgid,const void *ptr,size_t nbytes,int flag);

msgid:为msgget返回的消息队列ID值
ptr:为消息结构体mymesg指针
nbytes:为消息结构体mymesg里的字符数组mtext大小，sizeof(mtext)
flag:值可以为0、IPC_NOWAIT为0时，当消息队列满时，msgsnd将会阻塞，直到消息能写进消息队列或者消息队列被删除。为IPC_NOWAIT时，当消息队列满了，msgsnd函数将不会等待，会立即出错返回EAGAIN

返回值：成功返回0；错误返回-1
例如：msgsnd(id,(void *)&ckxmsg,512,0);

4.msgrcv函数从消息队列读取消息
所需头文件#include<sys/msg.h>
函数原型 ssize_t msgrcv(int msgid,void *ptr,size_t nbytes,long type,int flag);

msgid:为msgget返回的消息队列ID值
ptr:为消息结构体mymesg指针
nbytes:为消息结构体mymesg里的字符数组mtext大小，sizeof(mtext)
type:在结构体mymesg里我们定义了一个long int mtype，用于分别消息的类型
 type ==0 返回队列中的第一个消息
 type > 0 返回队列中消息类型为type的第一个消息
 type < 0 返回队列中消息类型值小于等于type绝对值的消息，如果这种消息有若干个，则取类型值最小的消息
flag:可以为0、IPC_NOWAIT、IPC_EXCEPT
 为0时，阻塞式接收消息，没有该类型的消息msgrcv函数一直阻塞等待
 为IPC_NOWAIT时，如果没有返回条件的消息调用立即返回，此时错误码为ENOMSG
 为IPC_EXCEPT时，与msgtype配合使用返回队列中第一个类型不为msgtype的消息
返回值：成功返回消息数据部分的长度；错误返回-1
例如：msgrcv(id,(void *)&ckxmsg,512,1,0);

5.msgctl函数对消息队列进行控制
简单的操作就是删除消息队列了，也可以获取和改变消息队列的状态
所需头文件#include<sys/msg.h>
函数原型int msgctl(int msgid, int cmd, struct msqid_ds *buf);
msgid就是msgget函数返回的消息队列ID
cmd有三个，常用删除消息队列的为IPC_RMID；IPC_STAT：取此队列的msqid_ds结构，并将它存放在buf指向的结构中；IPC_SET：改变消息队列的状态，把buf所指的msqid_ds结构中的uid、gid、mode复制到消息队列的msqid_ds结构内。（内核为每个消息队列维护着一个结构，结构名为msqid_ds，这里就不讲啦，里面存放着消息队列的大小，pid，存放时间等一些参数）
buf就是结构体msqid_ds

返回值：成功返回0；错误返回-1
例如：msgctl(id,IPC_RMID,NULL);删除id号的消息队列


函数原型int pthread_create(pthread_t *tidp,const pthread_attr_t *attr,(void*)(*start_rtn)(void*),void *arg);
      *tidp为线程标识符的指针,*attr用以设置线程属性,第三个参数是线程运行函数的起始地址,*arg为传入运行函数的参数
pthread_join等待一个线程结束
*/



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
       key_t key = ftok("tmp",66);
       int msqid = msgget(key, 0666 | IPC_CREAT);
       if( msqid == -1){
             printf("create failed");
             exit(-1);
       }

       while(1){
             sem_wait(mutex);
             printf("sender1 wirte :");
             scanf("%s", &buf);
             printf("\n");
             message.mtype = 1;
             if(strcmp(buf,"exit") == 0){
                     strcpy(message.mtext,"end1");
                     n = msgsnd(msqid, (void *)&message, 100, 0);
                     n = msgrcv(msqid, (void *)&message, 100, 2, 0);
                     printf("%s\n", message.mtext);
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
       char buf[MSG_MAX];
       sem_t *mutex = sem_open("mutex", O_CREAT | O_RDWR, 0666, 0);
       sem_t *sender2_over = sem_open("sender2_over", O_CREAT | O_RDWR, 0666, 0);
       sem_t *receive2_over = sem_open("receive2_over", O_CREAT | O_RDWR, 0666, 0);
       int msqid = msgget((key_t)8088, 0666 | IPC_CREAT);
       if( msqid == -1){
             printf("create failed");
             exit(-1);
       }

       while(1){
             sem_wait(mutex);
             printf("sender2 wirte :");
             scanf("%s", &buf);
             printf("\n");
             message.mtype = 1;
             if(strcmp(buf,"exit") == 0){
                     strcpy(message.mtext,"end2");
                     n = msgsnd(msqid, (void *)&message, 100, 0);
                     n = msgrcv(msqid, (void *)&message, 100, 3, 0);
                     printf("%s\n", message.mtext);

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
           int over1=0;
           int over2=0;
           struct mymsg message;
           char buf[MAX];
           key_t key = ftok("tmp",66);
           int msqid = msgget(key, 0666 | IPC_CREAT);

           if( msqid == -1){
               printf("create failed");
               exit(-1);
           }
             sleep(5);
           while(1){
                   n = msgrcv(msqid, (void *)&message, 100, 0, 0);
                   if(n > 0){
                         printf("\n receive %s\n", message.mtext);
                         if( strcmp(message.mtext,"end1") == 0 ){
                               message.mtype = 2;
                               strcpy(message.mtext,"over1");
                               n = msgsnd(msqid, (void *)&message, 100, 0);
                               sleep(3);
                               over1 = 1;
                         }else if( strcmp(message.mtext,"end2") == 0 ){
                               message.mtype = 3;
                               strcpy(message.mtext,"over2");
                               n = msgsnd(msqid, (void *)&message, 100, 0);
                               sleep(3);
                               over2 = 1;
                         }

                   }
                   if(over1==1 && over2==1){
                       msgctl(msqid, IPC_RMID, 0);
                       exit(0);
                   }
                   sleep(1);
           }

}





int main(){
           key_t key = ftok("tmp",66);
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
           return 0;
}
