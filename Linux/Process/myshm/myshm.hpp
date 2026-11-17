#ifndef __MYSHM_HPP__
#define __MYSHM_HPP__

#include <iostream>
#include <cstdio>
#include <cerrno>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string>
#include <cstring>

#define gCreater 1
#define gUser 2
#define gSize 4096

const std::string gpath = ".";
const int gproj_id = 123;

class Shm
{
private:
    // 生成唯一key
    key_t GetCommKey()
    {
        key_t key = ftok(_pathname.c_str(), _proj_id);
        if (key < 0)
        {
            perror("ftok");
        }
        return key;
    }

    // 创建/获取共享内存
    int ShmGet(key_t key, int size, int flag)
    {
        int shmid = shmget(key, size, flag);
        if (shmid < 0)
        {
            perror("shmget");
        }
        return shmid; // return shmid ，返回共享内存标识符
    }

    // 挂载共享内存
    void *AttachShm()
    {
        if (_addrshm != nullptr && _addrshm != (void*)-1)
        {
            DetachShm(_addrshm);
        }
        void *shmaddr = shmat(_shmid, nullptr, 0);
        if (shmaddr == (void*)-1)
        {
            perror("shmat");
            return nullptr;
        }
        std::cout << "attach shm..." << std::endl;
        return shmaddr;
    }

    // 卸载共享内存
    void DetachShm(void *shmaddr)
    {
        if (shmaddr == nullptr)
            return;
        shmdt(shmaddr);
        std::cout << "detach shm..." << std::endl;
    }

public:
    Shm(const std::string &pathname, int proj_id, int who)
        : _pathname(pathname), _proj_id(proj_id), _who(who), _addrshm(nullptr)
    {
        _key = GetCommKey();
        if (_who == gCreater)
            GetShmForUseCreate();
        else if (_who == gUser)
            GetShmForUse();

        _addrshm = AttachShm();
    }

    bool GetShmForUseCreate()
    {
        // Creater 负责创建
        if (_who == gCreater)
        {
            _shmid = shmget(_key, gSize, IPC_CREAT | IPC_EXCL | 0664);
            if (_shmid >= 0)
            {
                std::cout << "shm create done..." << std::endl;
                return true;
            }
        }
        return false;
    }

    bool GetShmForUse()
    {
        if (_who == gUser)
        {
            _shmid = shmget(_key, gSize, IPC_CREAT |0664);
            if (_shmid >= 0)
            {
                std::cout << "shm get done..." << std::endl;
                return true;
            }
        }
        return false;
    }

    void *Addr()
    {
        return _addrshm;
    }

    void Zero()
    {
        if (_addrshm)
        {
            memset(_addrshm, 0, gSize);
        }
    }

    ~Shm()
    {

        // 删除共享内存
        if (_who == gCreater)
        {
            int ret = shmctl(_shmid, IPC_RMID, nullptr);
            if (ret < 0)
            {
                perror("shmctl");
            }
            std::cout << "shm remove ..." << std::endl;
        }
        if (_addrshm && _addrshm != (void *)-1)
        {
            DetachShm(_addrshm);
        }
    }

    // 转十六进制
    std::string ToHex(key_t k)
    {
        char buf[128];
        snprintf(buf, sizeof(buf), "0x%x", k);
        return buf;
    }

private:
    key_t _key;
    int _shmid;
    int _proj_id;
    std::string _pathname;
    int _who;
    void *_addrshm;
};

#endif