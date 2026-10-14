#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>


int main()
{

	umask(0);
	int fd = open("myfile",O_RDONLY);
	if(fd < 0)
	{
		perror("error");
		return -1;
	}
	
	const char* message = "i like linux!\n";
	int len = strlen(message);
	char buf[1024];
	while(1)
	{
		ssize_t s = read(fd,buf,strlen(message));
		if(s > 0) 
			printf("%s",buf);
		else 
			break;
	}
	close(fd);
	return 0;
}
