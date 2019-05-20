#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/select.h>

#define ALIGN(x, exp)( (((x)+(1<<(exp))-1)>>(exp)) << (exp) )

const char m_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
#if 1

void base64_encode(const unsigned char *pSrc, int srclen, char *szOut)
{
     unsigned int buf;
     int i, nOffset;
     unsigned char *p = (unsigned char *)pSrc;
     char *r = szOut;
     while(p - pSrc < srclen)
     {
           // 3 * 8 ==> 4 * 6
           if(p - pSrc +3 < srclen)
           {
                 buf = (*p << 16) | (*(p + 1) << 8) | *(p + 2);
                 p += 3;
           }
           else
           {
                 buf = 0;
                 nOffset = p - pSrc;
                 while(p - pSrc < srclen)
                 {
                       buf <<= 8;
                       buf |= *p++;
                 }
                 buf <<= ((srclen - nOffset + 1) * 6) % 8;
                 for(i = srclen - nOffset; i >= 0; i--)
                       *r++ = m_alphabet[(buf >> (i * 6)) & 0x3F];
                 break;
           }
           *r++ = m_alphabet[(buf >> 18) & 0x3F];
           *r++ = m_alphabet[(buf >> 12) & 0x3F];
           *r++ = m_alphabet[(buf >> 6) & 0x3F];
           *r++ = m_alphabet[buf & 0x3F];
     }
     *r = 0;

	 { // add by hurong, ?懈鈻搊懈芯???懈芯=o?, 懈芯?懈芯懈鈻?斜鈹??懈鈻????
		 int sz = strlen(szOut);
		 int align_sz = ALIGN(sz, 2);
		 memset(r, '=', align_sz-sz);
		 szOut[align_sz] = 0;
	 }
}
#endif
#if 1
int file_size(char* filename)//获取文件名为filename的文件大小。
{
	FILE *fp;
	fp = fopen(filename, "rb+");//打开文件。
	int size;
	if(fp == NULL) // 打开文件失败
		return -1;
	fseek(fp, 0, SEEK_END);//定位文件指针到文件尾。
	size=ftell(fp);//获取文件指针偏移量，即文件大小。
	fclose(fp);//关闭文件。
	return size;
}
#endif
#if 0
  long filesize(FILE *stream);
  /*
   * int main(void)
  {
    FILE *stream;
    stream = fopen("MYFILE.TXT", "w+");
    fprintf(stream, "This is a test");
    printf("Filesize of MYFILE.TXT is %ld bytes\n", filesize(stream));
    fclose(stream);
    return 0;
  }*/
  long filesize(FILE *stream)
  {
    long curpos, length;
    curpos = ftell(stream);
    fseek(stream, 0L, SEEK_END);
    length = ftell(stream);
    fseek(stream, curpos, SEEK_SET);
    return length;
  }
#endif


#define cmd_head "wget -O "
#define cmd_log_path "result.log"
//#define	cmd_log_path "/home/fleety/share/result.txt"
#define cmd_json " --header=\"Content-Type: application/json\" --post-data=\"{\\\"license\\\":\\\""

#define cmd_img "\\\",\\\"imgData\\\":\\\""
#define cmd_mdtId "\\\",\\\"mdtId\\\":\\\""
#define cmd_1 "\\\"}\" "
#define cmd_ip "http://202.111.173.175:8082/"
#define cmd_face "seetaface.go?method=faceCompare 2>&1"


typedef struct
{
  unsigned char face_ret;
  unsigned char face_score;
}__attribute__((packed)) gb905_face_ret_t;


void send_face_data_info_to_ui(unsigned char face_ret,unsigned char face_num,unsigned char got_ret)
{
	printf("face_ret:%d face_num:%d got_ret:%d \n",face_ret,face_num,got_ret);
}

//{"result":"failure","score":19}
int  send_face_ret(unsigned char got_net_status)
{
 FILE *ret_fp;
 unsigned char key_result[] = "result";
 unsigned char ret_fail[] = "failure";
  unsigned char ret_suc[] = "success";
  
 unsigned char buf[80]={0};
 unsigned char *str_ret;
 unsigned char *str_ret2;
 unsigned char face_ret,face_score,temp_got_net;
 int len,num;

 	temp_got_net = got_net_status;
 	if(0 != temp_got_net)
	{
		ret_fp = fopen(cmd_log_path,"r");
		 if(ret_fp < 0)
		 {
			 printf("error send\n");
			 //result to ui
			 return 0;//<-----
		 }

		 fread(buf,1,sizeof(buf),ret_fp); //buf = {"result":"failure","score":19}
		 printf("buf::%s",buf);
		 fclose(ret_fp);
		//find result
		str_ret = strstr(buf,key_result);//str_ret = result":"failure","score":19}

		if(str_ret != NULL)
		{
			
			//find failure
			str_ret2 = strstr(str_ret,ret_fail);//str_ret2 = failure","score":19} or NULL

			if(str_ret2 == NULL)
			{
				//find success
				str_ret2 = strstr(str_ret,ret_suc);//str_ret2 = success","score":83} or NULL

				if(str_ret2 != NULL)
				{
				 face_ret = 1;
				}
				else
				{
				printf("asdfa not find result");
				temp_got_net = 0;
				send_face_data_info_to_ui(face_ret,face_score,temp_got_net);
				return 0;
				}
			}
			else
			{
			face_ret = 0;
			}

			
			//find :
			str_ret = strstr(str_ret2,":");//str_ret = :19} or NULL
			if(str_ret != NULL)
			{
			 num = atoi(str_ret+1);
			  face_score = num&0xff;
			}
			else
			{
			temp_got_net = 0;
			face_score = 0;
			printf("not find :");
			}
			printf("------------------- find :\n");
			send_face_data_info_to_ui(face_ret,face_score,temp_got_net);

		}
		else
		{
		printf("wda not find resul\n");
		temp_got_net = 0;
		send_face_data_info_to_ui(face_ret,face_score,temp_got_net);
		return 0;
		}
 	}
	else
	{
		temp_got_net = 0;
		printf("not find net");
		send_face_data_info_to_ui(face_ret,face_score,temp_got_net);
	}

	return 0;
}



int main(void)
{
	char path[] = "/home/fleety/share/test/20111212.jpg";
//	char path[] = "/home/fleety/share/test/u.jpg";
	char readbuf[90000]={0};
	char sendbuf[300000]={0};
	char cmd[300000] = {0};
	char mdtida[7] = "180884";
	char num[19] = "20111212";
	char buf1[1024]={0};
	char buf2[100]={0};
	int len,ret;
	int i;
	FILE *fp1;
	FILE *fp2;
	//FILE *ph;
	fp1 = fopen(path, "rb");//打开文件。
        int size,fd2;
	int face_ret;
        if(fp1 == NULL) // 打开文件失败
                return -1;
        fseek(fp1, 0, SEEK_END);//定位文件指针到文件尾。
	len = ftell(fp1);//获取文件指针偏移量，即文件大小。
	printf("--flet---len %d\n",len);
	//len = file_size(path);
        fseek(fp1, 0, SEEK_SET);//定位文件指针到文件尾。
	fread(readbuf,1,len,fp1);
	fclose(fp1);
	base64_encode(readbuf,len,sendbuf);
	printf("-----len %d\n",len);
	len = strlen(sendbuf);
	printf("--a---len %d\n",len);
	//printf("-----sendbuf-- %s\n",sendbuf);
	//sprintf(cmd,"%s20111212%s%s%s180884%s",cmd_head,cmd_2,sendbuf,cmd_3,cmd_4);
	sprintf(cmd,"%s%s%s%s%s%s%s%s%s%s%s",cmd_head,cmd_log_path,cmd_json,num,cmd_img,sendbuf,cmd_mdtId,mdtida,cmd_1,cmd_ip,cmd_face);
	
	fp2 = popen(cmd,"r");
	fd2 = fileno(fp2);
	fd_set fds;
	int key = 0;
	int maxfdp;
struct timeval timeout={10,0}; //select等待3微秒，3微秒轮询，要非阻塞就置0
//ph = fopen("rewget.txt","w+");
while(1)
{

	FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化
	FD_SET(fd2,&fds); //添加描述符
	timeout.tv_sec=10;
	timeout.tv_usec=0;//select函数会不断修改timeout的值，所以每次循环都应该重新赋值[windows不受此影响]
	maxfdp=fd2+1; //描述符最大值加1
	//printf("time:%d s  %d ms \n",timeout.tv_sec,timeout.tv_usec);
	switch(select(maxfdp,&fds,&fds,NULL,&timeout)) //select使用
	{
		case -1: 
			printf("erreor");
			key = 1;
			 break; //select错误，退出程序
		case 0:
			printf("-----1--\n");
			printf("timeout pp\n");
			system("killall wget -9");
			key = 1;
			 break; //再次轮询
		default:
        for(;;)
        {
#if 1
	fread(buf1,1,1024,fp2);
        //fgets(buf1,ret,fp2);
     //   printf("ret %d\n",ret);
        printf("buf1 %s\n",buf1);
 //       fwrite(buf1,1,ret,ph);
#endif
        if(feof(fp2))
                break;

        }
	face_ret = send_face_ret(1);	
	printf("------face_ret-- %d\n",face_ret);
	key = 1;
	printf("default\n");
		break;
	}
	if(key == 1)
	break;
}
 	if(!fp2){
         exit(EXIT_FAILURE);
         printf("----------");
 	}
 	
	pclose(fp2);
	
	//base64_encode(fread(fopen(path,'r',filesize(path))),,sendbuf);
	return 0;
}
