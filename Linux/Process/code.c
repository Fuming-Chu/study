#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{ 
	printf("process is running,only me! pid : %d \n",getpid());
	fork();
	printf("hello linux pid : %d , ppid : %d \n",getpid(),getppid());
	sleep(1);

//	pid_t id = getpid();
//	pid_t parent = getppid();
//	while(1)
//	{
//		printf("pid : %d  ppid : %d\n",id,parent);
//		sleep(1);
//	}

}
