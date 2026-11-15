#pragma once

#include <iostream>
#include <cstdio>
#include <cerrno>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//公共路径
// const std::string com_path = "./myfifo";


// int CreateNamedPipe(const std::string& path)
// {
//     int ret = mkfifo(path.c_str(),0664);
//     if(ret != 0 )
//     {
//         perror("mkfifo");
//     }
//     return ret;
// }


// int RemoveNamedPipe(const std::string & path)
// {
//     int ret = unlink(path.c_str());
//     if(ret != 0)
//     {
//         perror("unlink");
//     }
//     return ret;
// }