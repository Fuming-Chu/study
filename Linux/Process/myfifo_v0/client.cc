
#include "myNamedPipe.hpp"
const std::string mypath = "./myfifo";

int main()
{
    std::cout<<" creat mkfifo "<<std::endl;
    int wfd = open(mypath.c_str(),O_WRONLY);
    if(wfd < 0)
    {
        perror("open");
        return -1;
    }
    while(true)
    {
        std::cout << "please cin: ";
        std::string msg;
        std::getline(std::cin, msg);
        int n = write(wfd,msg.c_str(),msg.size());  //不要用&msg
        if(n < 0)
        {
            perror("write");
            break;
        }
    }
    close(wfd);
    unlink(mypath.c_str());

    return 0;
}