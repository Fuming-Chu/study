#pragma once

#include <iostream>
#include <cstdio>
#include <cerrno>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define Creater 1
#define User 2
#define DefaultFd -1
#define Read  O_RDONLY
#define Write O_WRONLY
#define Base_Size 4096

// 公共路径
const std::string com_path = "./myfifo";

class NamedPipe
{
private:
    bool OpenNamedPipe(int mode)
    {
        _fd = open(_myfifo_path.c_str(),mode);
        if(_fd < 0)
        {
            perror("open");
            return false;
        }
        return true;
    }

public:
    NamedPipe(const std::string &path, int who)
        : _myfifo_path(path), _flag(who),_fd(DefaultFd)
    {
        if (_flag == Creater)
        {
            int ret = mkfifo(_myfifo_path.c_str(), 0664);
            if (ret != 0)
            {
                perror("mkfifo");
            }
            std::cout<<"Creater create NamedPipe..."<<std::endl;
        }
    }

    bool OpenNamedPipeForRead(){ return OpenNamedPipe(Read);}
    bool OpenNamedPipeForWrite(){return OpenNamedPipe(Write);}


    //cosnt &:const std::string xxx 输入型参数 ，只是读值
    //* : 输出型参数，std::string*  out ,把值放到out里面负责输出
    //& : 输入输出型参数
    int ReadNamedPipe(std::string* out)
    {
        char buf[Base_Size];
        int n = read(_fd,buf,sizeof(buf)-1);
        if(n > 0)
        {
            std::cout<<"Read Named Pipe"<<std::endl;
            buf[n] = '\0';
            *out = buf;
        }
        return n;
    }
    int WriteNamedPipe(const std::string & in)
    {
        return write(_fd,in.c_str(),in.size());
    }


    ~NamedPipe()
    {
        if (_flag == Creater)
        {
            int ret = unlink(_myfifo_path.c_str());
            if (ret != 0)
            {
                perror("unlink");
            }
            std::cout<<"Creater remove NamedPipe..."<<std::endl;
        }
        if(_fd != DefaultFd) close(_fd);
    }

private:
    const std::string _myfifo_path;
    int _flag;  //负责识别身份
    int _fd;
};
