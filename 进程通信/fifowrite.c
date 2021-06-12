#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#define FIFO_NAME "/tmp/testfifo"

main()
{
    int fd;
 
    if((mkfifo(FIFO_NAME,0777)<0)&&(errno!=EEXIST))
    {
        printf("cannot create fifo...\n");
        exit(1);
    }
 
    //以阻塞型只写方式打开fifo
    fd=open(FIFO_NAME,O_WRONLY);
    int count = 0;
    while (1)
    {
        char buf[1024] = "A";
        write(fd,buf,sizeof(buf));
        count++;
        printf("the named pipe already write %d KB\n",count);
    
    }

}
 
