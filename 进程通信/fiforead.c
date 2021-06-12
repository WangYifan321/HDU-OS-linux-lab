//读进程
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#define FIFO_NAME "/tmp/testfifo"
 
main()
{
    char r_buf[1024];
    int  fd;
    int  r_num;
    int num=0;
 
// create or use pipe
    if((mkfifo(FIFO_NAME,0777)<0)&&(errno!=EEXIST))
    {
        printf("cannot create fifo...\n");
        exit(1);
    }
    //open pipefifo by only read and block
    fd=open(FIFO_NAME,O_RDONLY);
    if(fd==-1)
    {
        printf("open %s for read error\n");
        exit(1);
    }
 while(1){
    printf("please input how much KB you want to read(0<x<65),0KB represent exit\n");
    scanf("%d",&num);
    if(num>0 && num<=64)
    {
    printf("------------will read %d KB---------------------------------\n",num);
        while(num--){

    r_num=read(fd,r_buf,1024);
    printf("already read %d bytes, the content read is :%s\n",r_num,r_buf);
        }
        printf("-----------------------read over--------------------------------------\n");
    }
    else
    break;
 }
}
 
