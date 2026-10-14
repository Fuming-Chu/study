#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>


int main()
{

	umask(0);
	int fd = open("myfile",O_WRONLY | O_CREAT,0644);
	if(fd < 0)
	{
		perror("error");
		return -1;
	}
	
	int cnt = 5;
	const char* message = "i like linux!\n";
	int len = strlen(message);

	while(cnt--)
	{
		write(fd,message,len);

	}
	
	close(fd);
	return 0;
}
