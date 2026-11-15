
#include "myNamedPipe.hpp"


int main()
{
    NamedPipe fifo(com_path, User);
    while (true)
    {
        if (fifo.OpenNamedPipeForWrite())
        {
            std::cout << "Please cin :"<<std::endl;
            std::string msg;
            std::getline(std::cin, msg);
            fifo.WriteNamedPipe(msg);
        }
    }

    return 0;
}