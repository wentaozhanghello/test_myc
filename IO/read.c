#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(void)
{
	int fd,len;
	unsigned char path[40]= "/home/fleety/share/test/fileopen.txt";
	unsigned char buf[40];
	len = strlen(path)+1;
	printf("%s\n",path);
	fd = open(path,O_RDONLY);
	if(fd <0)
	{
		printf("fd :%d\n",fd);
		return 0;		
	}
	printf("%d\n",fd);
//	write(fd,path,len);
	read(fd,buf,len);
	printf("%s\n",buf);
	close(fd);
	return 0;
}
