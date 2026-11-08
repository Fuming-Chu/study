#include<iostream>
#include<unistd.h>
#include<cerrno>
#include<cstring>
#include<string>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

const int size = 1024;

//把子进程的pid发给父进程
std::string getOtherMessage()
{

    static int cnt = 0;
    std::string messageid = std::to_string(cnt);
    cnt++;
    pid_t self_id = getpid();   //获取pid
    
    std::string stringpid  = std::to_string(self_id);

    std::string message = "messageid: " ;
    message += messageid;
    message += " my pid is: ";
    message += stringpid;
    //message += "\n";

    return message;
}


//子进程写入数据
void SubProcessWrite(int wfd)
{
    int pipesize = 0;
    char c = 'A';
    std::string str = "I am a child process, I will write data to the pipe\n";
    while(true)
    {

        std::string info = str + getOtherMessage(); //这条消息就是子进程发个父进程的消息
        write(wfd,info.c_str(),info.size());  //写入管道，值得注意的是，因为c/cpp 混合写 cpp使用系统调用，系统调用使用c语言进行封装的

        //还需要注意写入管道的时候，有没有写入\0,有没有必要？
        //Q: 没有，没有必要

        //子进程如何发送？

        //我们知道管道本质也是文件
        sleep(1);

        // write(wfd,&c,1); //写入管道
        // std::cout << "pipesize:  " <<++pipesize<<" write character is : "<< c <<std::endl;
        // c++;
        // if(c == 'Z')
        // {
        //     break;
        // }
        // sleep(1);
        
    }
    std::cout<<"child quit..."<<std::endl;
}


//父进程读取数据

void FatherProcessRead(int rfd)
{

    char inbuf[size];   //c99,gnu g99
    while(true)
    {

        //父进程读取数据
        //从管道里面读取数据,read 从rfd里面读取数据
        ssize_t n = read(rfd,inbuf,sizeof(inbuf)-1); //这里-1的原因是子进程写入管道的时候没有过把\0写入,后来需要手动加上\0
        if(n > 0)
        {
            inbuf[n] = '\0'; //手动加上\0
            std::cout<<"father get message : "<<inbuf<<std::endl;
        }
        else if(n == 0)
        {

            //如果read返回值为0，表示写端关闭了，读到了文件结尾
            std::cout<<"client quit father get return value: "<<n<<std::endl;
            break;
        }
        else
        {
            std::cerr<<"father get message error, errno: "<<errno<<", errstring: "<<strerror(errno)<<std::endl;
            break;
        }
        

        //TODO
        sleep(1);
        break; //为了测试，父进程只读取一次数据就退出了

    }




}


int main() {


    //close(0); //验证之前的文件描述符

    //让我们对应的不同的进程看到同一份资源
    //1 .创建管道
    int pipefd[2];

    int n = pipe(pipefd); //输出型参数，rfd,wfd
    if(n != 0) 
    {
        std::cerr<< "errno" << errno <<"errstring : "<<strerror(errno)<<std::endl;
        return 1;
    }
    //pipefd[1]->1->写, pipefd[0]->0->读
    std::cout<<"pipefd[0]: "<<pipefd[0]<<",pipefd[1]: "<<pipefd[1]<<std::endl;

    sleep(1);

    //2 .创建子进程


    //3. 关闭不需要的文件描述符fd
    pid_t id = fork();

    if(id == 0)
    {
        sleep(1);
        std::cout<<"子进程关闭不需要的文件描述符fd, 准备发消息了"<<std::endl;
        //子进程
        close(pipefd[0]); //关闭读端

        SubProcessWrite(pipefd[1]); //子进程写入数据，向1里面写


        //在进程退出的时候可以关闭fd
        close(pipefd[1]); //关闭写端
        exit(0);
    }

    std::cout<<"父进程关闭不需要的文件描述符号fd,准备接收消息了"<<std::endl;

    sleep(1);
    //父进程
    close(pipefd[1]); //关闭写端
    FatherProcessRead(pipefd[0]); //父进程读取数据，向0里面读
    std::cout<<"father close read rfd"<<std::endl;

    sleep(5);
    close(pipefd[0]); //关闭读端

    int status = 0; // 位图结构，低7位表示子进程的退出信号，次低8位表示子进程的退出码
    pid_t rid = waitpid(id,&status,0);
    //pid_t rid = waitpid(id,nullptr,0);

    if(rid > 0)
    {
        std::cout << "wait child process done exit sig: " << (status&0x7f)<<std::endl;
        std::cout<<"wait child process done exit code(ign)) "<<((status>>8)&0xFF)<<std::endl;
        //std::cout<<"waitpid success, rid: "<<rid<<std::endl;
    }

    //close(pipefd[0]);//关闭读端
    return 0;
}