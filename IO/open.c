#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


int main(void)
{
	int fd;
	unsigned char path[40]= "/home/fleety/share/test/fileopen.txt";

	fd = open(path,O_RDWR|O_CREAT|O_TRUNC);
	if(fd <0)
	{
		printf("fd :%d\n",fd);
	}
	close(fd);
	return 0;
}
