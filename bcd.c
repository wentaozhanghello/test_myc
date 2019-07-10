#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int decimal2bcd(unsigned int dec,unsigned char *buf, int len)
{
	int i;
	unsigned temp;

	//DbgPrintf("dec = %d\r\n",dec);
	//DbgPrintf("len = %d\r\n",len);

	memset(buf,0x00,len);
	
	for(i=len-1;i>=0;i--)
	{
		if(dec == 0)
			break;
			
		temp = dec % 100;		
		buf[i] = ((temp/10) << 4) | ((temp % 10) & 0x0F);   
		dec /= 100;


		//DbgPrintf("temp = %d\r\n",temp);
		//DbgPrintf("dec = %d\r\n",dec);
		//DbgPrintf("val = %d\r\n",buf[i]);

	}
	
	return i+1;
}

/*
* bcd码转化为十进制
* @buf : 存放bcd的缓存
* @len : 存放bcd缓存的长度
* @return : 返回十进制的数据
*/
/*
unsigned int bcd2decimal(unsigned char *buf, int len)
{
	int	i;
	unsigned int tmp;
	unsigned int dec = 0;
	
	for(i=0;i<len;i++)
	{
		tmp = ((buf[i] >> 4) & 0x0F) * 10 + (buf[i]&0x0F);
		dec += tmp * power(100, len-1-i);    
	}

	return dec;
}
*/
int main(void)
{
	unsigned char bcd[10];
	int c;
	bcd[0]=0x19
	 
	c = bcd2decimal(&bcd[0],1)+100;//tran to year since 1900
	printf("c %d\n",c);
}