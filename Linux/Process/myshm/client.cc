#include "myshm.hpp"
#include "myNamedPipe.hpp"

int main()
{
    // 创建共享内存
    Shm myshm(gpath, gproj_id, gUser);
    myshm.Zero();

    char *shmaddr = (char *)myshm.Addr();

    //
    sleep(2);
    NamedPipe fifo(com_path, User);
    fifo.OpenNamedPipeForWrite();

    //测试例1
    while(true)
    {
        std::string tmp = "i am process A";
        std::string t = "wake up";
        strcpy(shmaddr,tmp.c_str());
        std::cout<<"client write to shm: "<<tmp.c_str()<<std::endl;
        fifo.WriteNamedPipe(tmp);
        sleep(3);
    }

    // 当成string  测试例2
    // char ch = 'A';
    // while (ch <= 'Z')
    // {
    //     shmaddr[ch - 'A'] = ch;

    //     std::string temp = "wakeup";
    //     std::cout << "add " << ch << " into Shm, " << "wakeup reader" << std::endl;
    //     fifo.WriteNamedPipe(temp);
    //     sleep(3);
    //     ch++;
    // }
    return 0;
}