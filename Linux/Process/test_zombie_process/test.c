#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	pid_t id = fork();
	if(id < 0)
	{
		perror("fork");
		return -1;
	}
	else if(id > 0)
	{
		printf("parent id : %d is sleeping...\n",getpid());
		sleep(10);
	}
	else
	{
		printf("child id : %d begin Z...\n",getpid());
		sleep(5);
		exit(EXIT_SUCCESS);
	}

	return 0;
}
