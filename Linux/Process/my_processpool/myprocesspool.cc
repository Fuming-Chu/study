#include<iostream>
#include<vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "mytask.hpp"

class Channel
{
public:

//构造函数
Channel(int wfd,pid_t id,const std::string &name)
    :_wfd(wfd),_subprocesspid(id),_name(name)
{}

~Channel(){}

//getter

int GetWfd(){ return _wfd;}
pid_t GetSubProcessID(){return _subprocesspid;}
std::string GetName(){ return _name;}

void CloseChannel(){close(_wfd);}

void Wait()
{
    pid_t rid = waitpid(_subprocesspid,nullptr,0);
    if(rid > 0)
    {
        std::cout<<"wait "<<rid<<" success "<<std::endl;
    }


}

private:
    int _wfd;
    pid_t _subprocesspid;
    std::string _name;
};

void work(int rfd)
{
    //printf("I am child process, read to work...\n");

    //child -- 执行任务
    while(true)
    {
        int cmd = 0;
        int n = read(rfd,&cmd,sizeof(cmd)); //拿到任务编号
        if(n == sizeof(int))
        {
            //指令有效
            std::cout<<"pid is "<<getpid()<<" handler task "<<std::endl;
            ExecuteTask(cmd);

        }
        else if(n == 0)
        {
            //写端全部关闭，退出子进程
            std::cout<<"sub process quit ..."<<std::endl;
            break;
        }
        else
        {
            perror("read");
            break;
        }


    }

}

void CreateChannelAndSub(int num,std::vector<Channel>* channels)
{
    //一个进程对应一个管道
    for(int i = 0;i < num;i++)
    {
        //1 创建管道
        int pipefd[2] = {0};
        int n = pipe(pipefd);   //man 2 pipe 可以手册查询详细的使用方法
        if(n  < 0) { exit(EXIT_FAILURE);}
        
        //2 创建子进程
        pid_t id = fork();
        if(id == 0)
        {
            //child -- read
            close(pipefd[1]);
            work(pipefd[0]);    //子进程读
            close(pipefd[0]);
            exit(EXIT_SUCCESS);

        }

        //父进程关闭读
        close(pipefd[0]);

        //3 构建一个channel名字
        std::string channel_name = "Channel-" + std::to_string(i);
        
        //子进程的pid和父进程的写端
        channels->push_back(Channel(pipefd[1],id,channel_name));
    }

}

int NextChannel(int channelsize)
{
    //轮询调度 0 1 2 3 4 ... channelsize-1(RR)
    static int next = 0;
    int channel = next;
    next++;
    next %= channelsize;
    return channel;
}

void SendTaskCommand(Channel & channel,int taskcmd)
{
    //发送任务
    write(channel.GetWfd(),&taskcmd,sizeof(taskcmd));

}

void ctrlProcessOnce(std::vector<Channel>& channels)
{
    //每一秒排发一个任务
    sleep(1);

    //选择任务
    int taskcmd = SelectTask();

    //选择一个管道和进程(轮询)
    int channel_index = NextChannel(channels.size());

    //发送任务
    SendTaskCommand(channels[channel_index],taskcmd);

    //测试任务发送
    std::cout<<"taskcmd: "<<taskcmd<<" channel : "<<channels[channel_index].GetName()<<" sub process:  "<<channels[channel_index].GetSubProcessID()<<std::endl;

}
void CtrlProcess(std::vector<Channel>& channels,int times = -1)
{
    if(times > 0)
    {
        while(times--)
        {
            ctrlProcessOnce(channels);
        }

    }
    else
    {
        while(true)
        {
            ctrlProcessOnce(channels);
        }

    }

}


void CleanUPChannel(std::vector<Channel>& channels)
{
    //回收管道和子进程需要先关闭所有写端 再回收子进程
    for(auto & channel : channels)
    {
        channel.CloseChannel();
    }

    for(auto& channel : channels)
    {
        channel.Wait();
    }

}

int main(int argc,char* argv[])
{


    //运行程序时，提示输入进程数(参数两个)
    //
    //argc：argument count，命令行参数个数（包含程序本身）
    //argv：argument vector，字符串数组，保存每一个参数
    if(argc != 2)
    {
        std::cerr<<"Usage: "<<argv[0]<<" process num"<<std::endl;
        return 1;
    }

    int num = std::stoi(argv[1]);   //拿到进程池中进程的数量

    //记载任务
    LoadTask();

    //使用vector封装Channel
    std::vector<Channel> channels;  //用于存放管道

    //3.1 创建管道和子进程
    CreateChannelAndSub(num,&channels);

    //3.2 通过channel控制子进程
    CtrlProcess(channels,num);

    //3.3 回收管道和子进程
    CleanUPChannel(channels);

    return 0;
}