#include<stdio.h>       // 标准输入输出头文件，包含printf等函数
#include <sys/types.h>   // 系统数据类型头文件，定义ssize_t、off_t等类型
#include <sys/stat.h>    // 文件状态相关，open等函数需要
#include <fcntl.h>       // 文件控制，open函数、O_*宏定义
#include <string.h>      // 字符串函数，strlen()
#include <unistd.h>     // Unix标准函数，read、write、close等函数

int main()
{
    char buf[1024];                  // 定义缓冲区，大小1024字节，存放读取到的数据
    ssize_t s = read(0, buf, sizeof(buf)); // 从标准输入fd=0读取数据到buf，返回读到的字节数
    if(s > 0){                       // 判断是否成功读到数据（读到字节大于0）
        buf[s] = 0;                  // 在读取末尾添加字符串结束符'\0'，转为C风格字符串
        write(1, buf, strlen(buf));  // fd=1标准输出，把buf内容输出到屏幕
        write(2, buf, strlen(buf));  // fd=2标准错误，同样输出一遍buf内容
    }
    return 0;                        // 程序正常退出，返回0
}