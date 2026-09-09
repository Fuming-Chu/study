#include<stdio.h>

int AddNu(int begin,int end)
{
	int n = 0;
	for(int i = begin;i <= end;i++)
	{
		n += i;
	}
	return n;
}
int main()
{
	int begin = 0;
	int end = 100;

	int result = AddNu(begin,end);
	printf("%d\n",result);
	
	return 0;
}
