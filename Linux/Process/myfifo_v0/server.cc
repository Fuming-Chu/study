
#include "myNamedPipe.hpp"

const std::string& mypath = "./myfifo";

int main()
{

    int ret = mkfifo(mypath.c_str(),0664);
    if(ret < 0 )
    {
        perror("mkfifo");
        return -1;
    }
    std::cout<<" creat mkfifo "<<std::endl;

    int rfd = open(mypath.c_str(),O_RDONLY);
    if(rfd < 0)
    {
        perror("open");
        return -1;
    }

    while(true)
    {
        char buf[128] = {0};
        ssize_t n = read(rfd, buf, sizeof(buf)-1);
        if (n > 0)
        {
            buf[n] = '\0';
            std::cout << "Say> "<<buf<<std::endl;
        }
        else if( n == 0)
        {
            std::cout<<"写端关闭"<<std::endl;
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }
    close(rfd);
    unlink(mypath.c_str());

    return 0;
}