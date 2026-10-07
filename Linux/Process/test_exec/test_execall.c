#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	char* const argv[] = {"ps","-ef",NULL};
    char* const envp[] = {"PATH=/bin:/usr/bin","TERM=console",NULL};

	execl("/bin/ps","ps","-ef",NULL);

	//带p的，可以使用环境变量PATH，无需写全路径
	execlp("ps","ps","-ef",NULL);

	//带e的，需要自己组装环境变量
	execle("ps","ps","-ef",NULL,envp);

	//带v的参数用数组
	execv("/bin/ps",argv);

	execvp("ps",argv);

	exit(0);

	return 0;
}
