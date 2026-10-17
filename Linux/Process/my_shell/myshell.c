#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>



#define SIZE 512
#define ZERO '\0'
#define NUM 32
#define SEP " "

// "ls -a -l -n > myfile.txt"
#define None_Redir 0
#define In_Redir 1
#define Out_Redir 2
#define App_Redir 3

#define SkipSpace(cmd,pos) do{\
	while(1){\
		if(isspace(cmd[pos])) pos++;\
		else break;\
	}\
}while(0)

int redir_type = None_Redir;
char* filename = NULL;

char cwd[SIZE*3];
char* sArgv[NUM];
int lastcode = 0;

//username
const char* GetUserName()
{
	const char* name = getenv("USER");
	if(name == NULL) return "None";
	return name;
}

//hostname
const char* Get_HostName()
{
	const char* hostname = getenv("HOSTNAME");
	if(hostname == NULL) return "None";
	return hostname;
}

//pwdname
const char* GetPwdName()
{
	const char* pwdname = getenv("PWD");
	if(pwdname == NULL) return "None";
	return pwdname;
}

void MakeCommandLine()
{
	char line[SIZE];
	const char* username = GetUserName();
	const char* hostname = Get_HostName();
	const char* pwdname = GetPwdName();

	snprintf(line,sizeof(line),"[%s@%s: %s]>",username,hostname,pwdname);
	printf("%s",line);
	fflush(stdout);
}

int GetUserCommand(char command[],size_t size)
{
	char *s = fgets(command,size,stdin);
	if(s == NULL) return -1;	//temp
	command[strlen(command)-1] = ZERO;
//	printf("echo : %s\n",command);

	//success return len
	return strlen(command);
}


void SplitUserCommand(char command[],size_t size)
{
	//"ls -a -l"  -> "ls" "-a" "-l"
	sArgv[0] = strtok(command,SEP);
	int index = 1;
	while(sArgv[index++] = strtok(NULL,SEP));
}

void ExecuteCommand()
{
	
	pid_t pid = fork();
	if(pid < 0 ) {perror("fork fail!\n");}
	else if(pid == 0)
	{
		//重定向设置
		if(filename != NULL)
		{
			if(redir_type == In_Redir)
			{
				int fd = open(filename,O_RDONLY);
				dup2(fd,0);	//输入重定向
			}
			else if(redir_type == Out_Redir)
			{
				int fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC,0666);
				dup2(fd,1);//输出重定向
			}
			else if(redir_type == App_Redir)
			{
				
				int fd = open(filename,O_WRONLY | O_CREAT | O_APPEND,0666);
				dup2(fd,1);//追加重定向
			}
			else{}
		}

		//child
		execvp(sArgv[0],sArgv);
		exit(errno);
	}
	else
	{
		//father
		int status = 0;
		pid_t rid = waitpid(pid,&status,0);
		if(rid > 0)
		{
			//获取退出码
			lastcode = WEXITSTATUS(status);
			if(lastcode != 0) printf("%s:%s:%d\n",sArgv[0],strerror(lastcode),lastcode);
		}
	}



}


const char* GetHome()
{
	const char* home = getenv("HOME");
	if(home == NULL){return "/";}
	return home;
}
int CheckBuildin()
{
	int yes = 0;
	const char *cmd = sArgv[0];
	if(strcmp(cmd,"cd") == 0)
	{
		yes = 1;
		
		//Cd()
		const char* path = sArgv[1];
		if(path == NULL)
		{
			//cd 直接返回root路径
			path = GetHome();
		}
		//路径存在 chdir
		chdir(path);
		
		//刷新环境变量
		char temp[SIZE*2];
		getcwd(temp,sizeof(temp));
		snprintf(cwd,sizeof(cwd),"PWD=%s",temp);
		putenv(cwd);
	}
	else if(0 == strcmp(cmd,"echo") && 0 == strcmp(sArgv[1],"$?"))
	{
		yes = 1;
		printf("%d\n",lastcode);
		lastcode = 0;
	}

	return yes;

}

void CheckRedir(char cmd[])
{

	int pos = 0;
	int end = strlen(cmd);

	while(pos < end)
	{

		if(cmd[pos] == '>')
		{
			if(cmd[pos+1] == '>')
			{

				cmd[pos++] = 0;
				pos++;
				redir_type = App_Redir;
				SkipSpace(cmd,pos);
				filename = cmd + pos;
			}
			else 
			{
				cmd[pos++] = 0;
				redir_type = Out_Redir;
				SkipSpace(cmd,pos);
				filename = cmd + pos;

			}
		}
		else if(cmd[pos] == '<')
		{
			cmd[pos] = 0; //分割
			redir_type = In_Redir;
			SkipSpace(cmd,pos);	//跳过空格
			filename = cmd + pos;	//拿到文件名的起始地址
		}
		else 
		{
			pos++;
		}

	}
}


int main()
{
	int q = 0;
	while(!q)
	{
		//0 重置		
		redir_type = None_Redir;
		filename = NULL;
	    //1 获取命令行
	    MakeCommandLine();


	    //2 获取用户命令字符串
	    char usercommand[SIZE];
	    int n = GetUserCommand(usercommand,sizeof(usercommand));
	    if(n <= 0) return 1; //tmp
	    
		//2.1 checkredir
		CheckRedir(usercommand);
		
		//debug
	//	printf("cmd : %s\n",usercommand);
	//	printf("redir : %d\n",redir_type);
	//	printf("filename : %s\n",filename);

	    //3 分割命令行字符串
	    SplitUserCommand(usercommand,sizeof(usercommand));
        
		//4 检查命令是否是内建命令
		n = CheckBuildin();
		if(n) continue;
	    //n 执行命令
	    ExecuteCommand();

	}
    return 0;
}
