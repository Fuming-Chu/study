#include<iostream>
#include<unistd.h>
#include<string>
#include<vector>
#include<sys/types.h>
#include<sys/wait.h>

#include "Task.hpp"

// //child进程的工作
// void work(int rfd)
// {

//     while(true) 
//     {
//         int command = 0;
//         int n = read(rfd,&command,sizeof(command));
//         if(n == sizeof(int))
//         {
//             std::cout<<"pid is "<<getpid()<<" handler task "<<std::endl;
//             ExecuteTask(command);
//         }
//         else if(n == 0 )
//         {
//             std::cout<<"sub process : "<<getpid() << " quit ... "<<std::endl;
//             break;
//         }
//         //sleep(1);
//     }
// }


//父进程视角下的管道，使用Channel类来封装管道的读写端口和子进程的pid
class Channel
{
public:
    Channel(int wfd ,pid_t id , const std::string &name)
        :_wfd(wfd),_subprocessid(id),_name(name)
    {
        
    }


    ~Channel(){}

    void CloseChannel(){ close(_wfd);}

    void Wait()
    {
        pid_t rid = waitpid(_subprocessid,nullptr,0);
        if(rid > 0)
        {
            std::cout<<"wait "<< rid <<" success "<<std::endl;
        }

    }

    int GetWfd(){return _wfd;}
    pid_t GetProcessId(){return _subprocessid;}
    std::string GetName(){return _name;}

private:
    int _wfd;
    pid_t _subprocessid;
    std::string _name;
};

// 形参类型和命名规范
// const & : 输出
// & ：输入输出型参数
// * ： 输出型参数

// task_t task : 回调函数 -- 方便解耦
void CreateChannelAndSub(int num,std::vector<Channel> * channels,task_t task)
{
    for(int i = 0; i< num; i++)
    {
        //1 创建管道
        int pipefd[2] = {0};
        int n = pipe(pipefd);
        if(n < 0) exit(1);

        pid_t id = fork();
        if(id == 0)
        {

            //child -- read
            close(pipefd[1]);
            //v1 借助管道
            //work(pipefd[0]);

            //v2 , 使用系统调用重定向
            dup2(pipefd[0],0);  //将管道的读端，重定向到标准输入
            work();
            //task();
            close(pipefd[0]);
            exit(0);
        }

        //3 构建一个channel名
        std::string channel_name = "Channel-" + std::to_string(i);
        //父进程关闭读
        close(pipefd[0]);   //关闭读

        //子进程的pid和父进程的写端
        channels->push_back(Channel(pipefd[1],id,channel_name));
    }

}

int NextChannel(int channelnum)
{
    static int next = 0;
    int channel = next;
    next++;
    next %= channelnum;
    return channel;

}

void SendTaskCommand(Channel & channel,int taskcommand)
{
    write(channel.GetWfd(),&taskcommand,sizeof(taskcommand));
    //sleep(3);
}

void ctrlProcessOnce(std::vector<Channel>& channels)
{

    //每隔1秒派任务
    sleep(1);
    //a 选择一个任务
    int taskcommand = SelectTask();

    //b 选择一个信道和进程
    int channel_index = NextChannel(channels.size());

    //c 发送任务
    SendTaskCommand(channels[channel_index],taskcommand);
    std::cout<<std::endl;
    std::cout<<"taskcommand: "<<taskcommand<<" channel:  "<< channels[channel_index].GetName() <<"  sub process:  "<< channels[channel_index].GetProcessId() <<std::endl;

}

void ctrlProcess(std::vector<Channel> &channels, int times = -1)
{
    if (times > 0)
    {

        while (times--)
        {
            ctrlProcessOnce(channels);
        }
    }
    else
    {
        while (true)
        {
            ctrlProcessOnce(channels);
        }
    }
}

void CleanUpChannel(std::vector<Channel>& channels)
{
    //a 关闭所有的写端
    //b 回收子进程
    for(auto& channel : channels)
    {
        channel.CloseChannel();
    }

    //值得注意的是，所有子进程关闭了，父进程将所有子进程进行回收
    //因为之前fork，所有父进程要回收子进程
    for(auto& channel : channels)
    {
        channel.Wait();
    }
}

// 想让进程池 有5个进程
int main(int argc, char* argv[])
{
    //提示输入进程数
    if(argc != 2)
    {
        std::cerr<<"Usage: "<<argv[0]<<" processnum"<<std::endl;
        return 1;
    }

    int num = std::stoi(argv[1]);   //拿到进程池中进程的数量
    LoadTask();

    std::vector<Channel> channels; //存放所有的管道
    //1 创建信道和子进程
    CreateChannelAndSub(num,&channels,work);

    //2 通过channel控制子进程
    ctrlProcess(channels,5);

    //3 回收管道和子进程
    CleanUpChannel(channels);

    //test
    // for(auto& channel : channels)
    // {

    //     std::cout<<"============================================"<<std::endl;
    //     std::cout<<channel.GetName()<<std::endl;
    //     std::cout<<channel.GetWfd()<<std::endl;
    //     std::cout<<channel.GetProcessId()<<std::endl;
    // }



    //sleep(100);
    return 0;
}