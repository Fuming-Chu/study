
#include "myNamedPipe.hpp"

int main()
{
    NamedPipe fifo(com_path, Creater);
    // fifo.OpenNamedPipe(Read);

    if (fifo.OpenNamedPipeForRead())
    {
        while (true)
        {
            std::string msg;
            int n = fifo.ReadNamedPipe(&msg);
            if (n > 0)
            {
                std::cout << "Client Say:" << msg << std::endl;
            }
            else if (n == 0)
            {
                std::cout << "client close " << std::endl;
                break;
            }
            else
            {
                perror("read");
                break;
            }
        }
    }

    return 0;
}