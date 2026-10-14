#include<stdio.h>
#include<string.h>


int main()
{

	//1 打开文件：w+ 模式支持读写，文件不存在则创建，存在则清空原有内容
	FILE* fp = fopen("bite.txt","w+");
	
	if(fp == NULL) 
	{
		perror("fopne error\n");
	}

	const char* message = "linux so easy!";
	
	//2 向文件写入字符串
	//数据首地址，单块字节数，块数，文件流指针
	fwrite(message,strlen(message),1,fp);

	//3 移动文件指针到文件开头（写入后指针在末尾，必须偏移从能从头读取）
	//SEEK_SET 表示从文件起始位置偏移0字节
	fseek(fp,0,SEEK_SET);

	char r_buf[1024];
	//4 读取文件到缓冲区，预留一个字节放字符串结束符号
	size_t r_cnt = fread(r_buf,1,sizeof(r_buf)-1,fp);
	r_buf[r_cnt] = '\0';

	//5 打印
	printf("%s\n",r_buf);

	//6 关闭文件流，释放资源
	fclose(fp);
	return 0;
}
