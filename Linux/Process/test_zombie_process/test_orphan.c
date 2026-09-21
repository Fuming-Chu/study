#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

int main()
{
	pid_t id = fork();
	if(id < 0)
	{
		perror("failed");
		return 1;
	}
	else if(id == 0)
	{
		setsid();//创建一个新的会话，脱离当前终端和Subreaper的控制
		printf("child pid : %d ...\n",getpid());
		sleep(30);
	}
	else
	{
		printf("parent pid : %d ...\n",getpid());
		sleep(5);
		exit(0);
	}

	return 0;
}
