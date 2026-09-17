#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

int main(int atgc,char* argv[],char* env[])
{
	char* path = getenv("PATH");
	if(path == NULL) return 1;
	printf("path : %s\n",path);
	return 0;
}
