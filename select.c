#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define		UART_TEST_DATA_LEN		64
#define		MAX_TEMP_BUFF_SIZE		2048

int main(void)
{
	fd_set fds;
	int uart_id;
	int size,select_ret,maxfdp;
//	FleetyUartMngr *uart_mngr;
	unsigned char temp_buff[MAX_TEMP_BUFF_SIZE];
	int offset =0;
	int ret = 0;
	
//	uart_mngr = &uart_mngr_sync_list_test[index];
//	printf("uart rcv test id =%d \r\n",uart_mngr->uart_id);
	uart_id = open("/home/fleety/share/test/fileopen.txt",O_RDWR| O_NOCTTY);
	struct timeval timeout={5,0};
	FD_ZERO(&fds); //
	//FD_SET(uart_mngr->uart_id,&fds);
	//maxfdp=uart_mngr->uart_id+1;
	FD_SET(uart_id,&fds);
	maxfdp=uart_id+1;
	while(1)//size > 0
	{
	 fd_set rdset = fds;
	 select_ret = select(maxfdp,&rdset,NULL,NULL,&timeout);
	 if(select_ret < 0)
	 {
		printf("popen erreor\n");
		return 0;
	 }
	 else if(0 == select_ret)
	 {
		printf("popen timeout pp\n");
		return 0;
	 }
	 else
	 {
		printf("popen default\n");
		if(FD_ISSET(uart_id, &rdset))
		{
			ret = read(uart_id,temp_buff,UART_TEST_DATA_LEN);
			printf("popen read  ret :%d\n",ret);
			if(ret > 0)
			{
	 		 offset += ret;
	 		 if(offset == UART_TEST_DATA_LEN)
			 {
			 printf("cccc read ret :%d ,offset :%d \n",ret,offset);
			 return 1;
			 }
			printf("read ret :%d ,offset :%d \n",ret,offset);
			}
			else
			{
			 if(offset == UART_TEST_DATA_LEN)
			 {
			  printf("ee read ret :%d ,offset :%d \n",ret,offset);
			  return 1;
			 }
			return 0;
		        }
		}
	}
 }

	return -1;
}
