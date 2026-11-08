#pragma once 
#include<iostream>
#include<ctime>
#include<cstdlib>

#define TaskNum 3

typedef void (*task_t)();   //给这种 函数指针类型 起名字为 task_t类型
//using task_t = void(*)();   //c++11以及之后支持

task_t tasks[TaskNum];  //定义一个函数指针数组



void Print() { std::cout<<"Task 1 Print ..."<<std::endl;}
void DownLoad() { std::cout<<"Task 2 DownLoad ..."<<std::endl;}
void Flush() { std::cout<<"Task 3  Flush..."<<std::endl;}

void LoadTask()
{
    //生成随机种子
    srand(time(nullptr) ^ getpid()^17777);
    tasks[0] = Print;
    tasks[1] = DownLoad;
    tasks[2] = Flush;
}

int SelectTask() {return rand() % TaskNum;} //任务 0 1 2

void ExecuteTask(int cmdid)
{
    //调用函数指针
    if(cmdid < 0 || cmdid > 2)
    {
        return;
    }
    else
    {
        tasks[cmdid]();
    }
}