#include<iostream>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include<cstdlib>

const size_t SIZE = 1024;
const char* msg = "i am father";
// 先实现一版匿名管道下，父进程写，子进程读
int main()
{

    char buf[SIZE];
    //1 先创建管道
    int fd[2];  // fd[0] 读 ， fd[1] 写


    if(pipe(fd) == -1) { perror("make pipe");  exit(EXIT_FAILURE);}

    //2 创建子进程
    pid_t pid = fork();
    if(pid < 0) 
    { 
        perror("fork"); exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        //child

        //子进程关闭写,子进程读管道
        close(fd[1]);
        ssize_t n = read(fd[0],buf,sizeof(buf)-1);  //这里read只读原始字节，不会自动追加\0
        if(n > 0)
        {
            buf[n] = '\0';
            write(1,buf,n);
            write(1,"\n",1);
            

        }
        //关闭读
        close(fd[0]);
    }
    else
    {
        //father
        //父进程关闭读，负责写
        close(fd[0]);
        write(fd[1],msg,strlen(msg));

        //写完关闭写
        close(fd[1]);

        //父进程回收子进程，避免僵尸进程
        pid_t rid = waitpid(pid,nullptr,0);
        if(rid > 0) 
        {
            std::cout<<"wait success"<<std::endl;
        }

    }

    return 0;
}