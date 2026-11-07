#pragma once

#include <iostream>
#include<ctime>
#include<cstdlib>


#define TaskNum 3

typedef void (*task_t) ();  //task_t 函数指针类型


void Print()
{

    std::cout<<"I am Print Task"<<std::endl;

}

void DownLoad()
{

    std::cout<<"I am downlaod task" <<std::endl;
}

void Flush()
{

    std::cout<<"I am flush task"<<std::endl;
}

task_t tasks[TaskNum];

void LoadTask()
{
    srand(time(nullptr) ^ getpid() ^ 17777);
    tasks[0] = Print;
    tasks[1] = DownLoad;
    tasks[2] = Flush;

}

void ExecuteTask(int number)
{
    if(number < 0 || number > 2) return;
    {
        tasks[number](); //通过函数指针调用任务
    }
}

int SelectTask()
{
    return rand() % TaskNum;
}


//优化v2 ,使用系统调用 重定向
//child进程的工作
void work()
{

    while(true) 
    {
        int command = 0;
        int n = read(0,&command,sizeof(command));
        if(n == sizeof(int))
        {
            std::cout<<"pid is "<<getpid()<<" handler task "<<std::endl;
            ExecuteTask(command);
        }
        else if(n == 0 )
        {
            std::cout<<"sub process : "<<getpid() << " quit ... "<<std::endl;
            break;
        }
        //sleep(1);
    }
}