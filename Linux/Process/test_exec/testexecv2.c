#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	//多进程场景
	printf("testexec ... begin!\n");
	pid_t id = fork();
	if(id == 0)
	{
		sleep(2);
		execl("/usr/bin/ls","ls","-a","-l",NULL);
		exit(1);
	}
	int status = 0;
	pid_t rid = waitpid(id,&status,0);
	if(rid > 0)
	{
		printf("father wait success, child exit code: %d\n",WEXITSTATUS(status));
	}
	printf("end!\n");

	return 0;

}
