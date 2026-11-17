#include "myshm.hpp"
#include "myNamedPipe.hpp"

int main()
{

    //创建共享内存
    Shm myshm(gpath,gproj_id,gCreater);

    char* shmaddr = (char*)myshm.Addr();

    //创建管道 读端
    NamedPipe fifo(com_path,Creater);
    fifo.OpenNamedPipeForRead();

    while(true)
    {
        std::string tmp;
        int n = fifo.ReadNamedPipe(&tmp);
        if(n <= 0) break;
        sleep(1);
        std::cout<<"process B receive shm content: "<<shmaddr<<std::endl;
    }
    return 0;
}