#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	printf("testexec ... begin!\n");
	execl("/usr/bin/lssss","ls","-l","-a",NULL);
	printf("end!\n");

	return 0;

}
