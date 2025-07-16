#include <unistd.h>
#include<sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>


int main()
{
    int fds[2];
    char buffer[7] = {0};
    int rc = 0;


    //判断打开文件是否正确
    if((fds[0] = open("hello1",O_RDWR | O_CREAT,0666)) < 0){
        perror("Open hello1 failed");
    }
    //判断打开文件是否正确
    if((fds[1] = open("hello2",O_RDWR | O_CREAT,0666)) < 0){
        perror("Open hello2 failed");
    }

    //向文件2写入"hello"
    if((rc = write(fds[1],"hello!\n",7))){
        printf("rc=%d\n",rc);
    }
    lseek(fds[1],0,SEEK_SET);

    int fd_max = (fds[0] > fds[1]) ? fds[0] : fds[1];

    fd_set inset1;
    fd_set inset2;
    struct timeval tv;
    //初始化tv
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    //初始化写合集inset1
    FD_ZERO(&inset1);
    FD_SET(fds[0],&inset1);

    //初始化读合集inset2
    FD_ZERO(&inset2);
    FD_SET(fds[1],&inset2);

    while(FD_ISSET(fds[0],&inset1) || FD_ISSET(fds[1],&inset2)){

        //初始化tv结构体
        struct timeval tv;
        tv.tv_sec=2;
        tv.tv_usec=0;

        //建立临时合集，防止select()函数修改合集
        fd_set temp_read_set = inset2;
        fd_set temp_write_set = inset1;
        
        //使用select对io进行复用
        if(select(fd_max+1,&temp_write_set,&temp_read_set,NULL,&tv) < 0){
            perror("select error\n");
        }
        else{
            //判断读合集是否准备就绪
            if(FD_ISSET(fds[1],&temp_read_set)){
                //判断读操作是否正确
                rc = read(fds[1],buffer,7);
                if(rc > 0){
                    buffer[rc] = 0;
                    printf("rc=%d,buffer=%s",rc,buffer);
                }
                else{
                    perror("read failed\n");
                }
                lseek(fds[1], 0, SEEK_SET);
            }

            //判断写合集是否准备就绪
            if(FD_ISSET(fds[0],&temp_write_set) > 0){
                //判断写操作是否正确
                if((rc = write(fds[0],buffer,7))){
                    buffer[rc] = 0;
                    printf("rc=%d,write:%s",rc,buffer);
                }
                else{
                    perror("write failed\n");
                }
                sleep(5);
            }
        }

    }
    exit(0);
}