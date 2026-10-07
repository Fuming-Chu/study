#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main()
{
	pid_t pid = fork();

	if(pid < 0)
	{
		perror("fork error!");
		return 1;
	}
	else if(pid == 0)
	{
		//child
		printf("i am child process\n");
		sleep(5);
		return 10;
	}
	else
	{
		//父进程，等待子进程
		int status = 0;
		waitpid(pid,&status,0);	//阻塞等待指定pid子进程
		
		//解析status
		printf("子进程退出信息\n");
		if(WIFEXITED(status))
		{
			printf("子进程正常退出，退出码：%d\n",WEXITSTATUS(status));
		}
		if(WIFSIGNALED(status))
		{
			printf("子进程被信号终止，编号：%d\n",WTERMSIG(status));
		}
		
		//core dump标志位
		printf("core dump标志：%d\n",WCOREDUMP(status));

		return 0;


	}
		

}
