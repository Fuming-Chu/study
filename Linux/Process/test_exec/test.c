#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	pid_t pid = fork();

	if(pid < 0 )
	{
		perror("fork failed!\n");
		return 11;
	}
	else if(pid == 0)
	{
		//child
		printf("execl......\n");
		execl("/usr/bin/ls","ls","-l","-a",NULL);
		exit(1);
	}
	int status = 0;
	pid_t ret = waitpid(pid,&status,0);
	if(ret > 0)
	{
		printf("father wait success,child return code is :%d\n",WEXITSTATUS(status));
	}

	printf("testexec end\n");
	



	return 0;
}
