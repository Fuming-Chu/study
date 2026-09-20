#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	pid_t id;
	printf("before fork id : %d addr_id : %p\n",id,&id);
	
//	pid_t id = fork();
	id = fork();
	if(id == 0)
	{
		while(1)
		{
			printf("I am child process,pid : %d , id : %d, %p\n",getpid(),id,&id);
			sleep(1);
		}
	}
	else if(id > 0)
	{
		while(1)
		{
			printf("I am parent, id : %d, %p\n",id,&id);
			sleep(1);
		}
	}


	return 0;
}
