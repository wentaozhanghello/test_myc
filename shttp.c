#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define ALIGN(x, exp)( (((x)+(1<<(exp))-1)>>(exp)) << (exp) )

#define		UP_PIC_HTTP 	"循环测试"
#define		UP_PIC_HTTP_ING 	"上传"
#define		NO_PIC_HTTP_ING 	"没有照片"
#define		UP_PIC_HTTP_ERROR 	"出错"
#define		UP_PIC_HTTP_TIMEOUT 	"超时"
/************************************************************************
base64 鈻捬€鈹?
************************************************************************/

const char m_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// Base64 鈻捬€鈹嬧?鈺欌敎鈻?
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
// Base64 鈺溠傗敩褘
void base64_decode(const char *szCoded, unsigned char *pOut)
{      
     short nDecTab[256];
     short i;
     unsigned int buf;
     int nOffset, len = strlen(szCoded);
     char *p = (char *)szCoded;      
     unsigned char *r = pOut;

     memset(nDecTab, -1, sizeof(short) * 256);
     for(i = 0; i < 64; i++)
     {
           nDecTab[m_alphabet[i]] = i;
     }
     nDecTab['='] = -1;
     while(*p)
     {      
           // 4 * 6 ==> 3 * 8
           if(p + 4 - szCoded <= len)
           {
                 buf = ((nDecTab[*p] & 0x3F) << 18) | ((nDecTab[*(p + 1)] & 0x3F) << 12) |
                       ((nDecTab[*(p + 2)] & 0x3F) << 6) | (nDecTab[*(p + 3)] & 0x3F);
                 p += 4;
           }
           else
           {
                 nOffset = p - szCoded;
                 if(nOffset != len)
                 {
                       buf = 0;
                       while(*p)
                       {
                             buf <<= 6;
                             buf |= (nDecTab[*p++] & 0x3F);      
                       }
                       buf >>= ((len - nOffset) * 6) % 8;
                       for(i = ((len - nOffset) * 6) >> 3; i > 0; i--)
                             *r++ = (buf >> ((i - 1) << 3)) & 0xFF;
                       break;
                 }                  
           }
           *r++ = (buf >> 16) & 0xFF;
           *r++ = (buf >> 8) & 0xFF;
           *r++ = buf & 0xFF;            
     }      
     *r = 0;

}

#if 0
wget 
-O result.log 
--header="Content-Type: application/json" 
--post-data="{\"license\":\"123456789012345678\",\"imgData\":\"ABCDEFG\",\"mdtId\":\"180884\"}" 
http://202.111.173.175:8082/seetaface.go?method=faceCompare

wget -O result.log --header="Content-Type: application/json" --post-data="{\"license\":\"123456789012345678\",\"imgData\":\"ABCDEFG\",\"mdtId\":\"180884\"}" http://202.111.173.175:8082/seetaface.go?method=faceCompare

#endif

#define cmd_head "wget -O "
//#define cmd_log_path "/mnt/nand/face_result.log"
#define cmd_log_path "/home/fleety/share/test/face_result.log"
#define cmd_json_license " --header=\"Content-Type: application/json\" --post-data=\"{\\\"license\\\":\\\""
#define cmd_img "\\\",\\\"imgData\\\":\\\""
#define cmd_mdtId "\\\",\\\"mdtId\\\":\\\""
#define cmd_json_end  "\\\"}\" "
#define cmd_server_baseaddr  "http://202.111.173.175:8082/"
//#define cmd_server_baseaddr  "http://202.111.173.175:8083/"

/*must have 2>&1 !!!!!!*/
#define cmd_face "seetaface.go?method=faceCompare 2>&1"


//{"result":"failure","score":19}
int  prase_json_face_ret_data(unsigned char got_net_status)
{
	FILE *ret_fp;
	unsigned char key_result[] = "result";
	unsigned char ret_fail[] = "failure";
	unsigned char ret_suc[] = "success";
	unsigned char buf[80]={0};

	unsigned char *str_ret;
	unsigned char *str_ret2;
	unsigned char face_ret =0;
	unsigned char face_score =0;
	unsigned char temp_got_net =0;
	int len,num;

 	temp_got_net = got_net_status;
 	if(0 == temp_got_net)
	{
		printf("not find net");
		printf("a %d %d %d",face_ret,face_score,temp_got_net);
		return 0;
	}
	
		ret_fp = fopen(cmd_log_path,"r");
		if(ret_fp < 0)
		{
			printf("earror send\n");
			//result to ui
			return 0;
		}

		fread(buf,1,sizeof(buf),ret_fp); //buf = {"result":"failure","score":19}
		printf("buf::%s\n",buf);
		fclose(ret_fp);
		//find result
		str_ret = strstr(buf,key_result);//str_ret = result":"failure","score":19}

		if(str_ret == NULL)
		{
			printf("wda not find resul\n");
			temp_got_net = 0;
		printf("a %d %d %d",face_ret,face_score,temp_got_net);
			return 0;
		}


		//find success
		str_ret2 = strstr(str_ret,ret_suc);//str_ret2 = success","score":83} or NULL
		if(str_ret2 == NULL)
		{
			//find failure
			str_ret2 = strstr(str_ret,ret_fail);//str_ret2 = failure","score":19} or NULL
			if(str_ret2 == NULL)
			{
				printf("--- not find result");
				temp_got_net = 0;
		printf("a %d %d %d",face_ret,face_score,temp_got_net);
				return 0;
			}
			face_ret = 0;
		}
		else
		{
			face_ret = 1;
		}
		

		//find :
		str_ret = strstr(str_ret2,":");//str_ret = :19} or NULL
		if(str_ret != NULL)
		{
			num = atoi(str_ret+1);
			face_score = num&0xff;
			printf(" ---- find");
		}
		else
		{
			face_score = 0;
			temp_got_net = 0;
			printf(" ':' not find");
		}
		printf("a %d %d %d",face_ret,face_score,temp_got_net);

	return 0;
}
int read_pic_file_to_base64_code(char *send_datas,unsigned char *name)
{
	FILE *photo_fp;
	char *readbuf;
//	char path[128] = {0};
	char readpicbuf[100000]={0};
	int len;


//	len = strlen(name); 
//	memcpy(path,name,len);
//	photo_fp = fopen(path, "rb");//打开文件。
	photo_fp = fopen(name, "rb");//打开文件。
	
	if(photo_fp == NULL) // 打开文件失败
	{
		printf("---fp1--open-feil----\n");
		return 1;
	}
		
	fseek(photo_fp, 0, SEEK_END);//定位文件指针到文件尾。
	len = ftell(photo_fp);//获取文件指针偏移量，即文件大小。
	fseek(photo_fp, 0, SEEK_SET);//定位文件指针到文件头。
	if(len > sizeof(readpicbuf))
	{
		printf("-pic-too-big---\n");
		fclose(photo_fp);
		return 1;
	}
	fread(readpicbuf,1,len,photo_fp);
	fclose(photo_fp);

	base64_encode(readpicbuf,len,send_datas);

  return 0;
}

int wget_post_pic_func(unsigned char *buf,unsigned short len,int ret_wget)
{
        FILE *popen_fp;
	 fd_set fds;

	int popen_fd,maxfdp,no_pic,select_ret;
	int mtd_str_offset=0;
	int buf_offset=0;
	int key = 0;
	int len_picname;

	char tem_buf[1024] = {0};
	char sendbuf[300000]={0};
	char cmd[310000] = {0};
	char temp_mtd_id_str[7]={0};
	unsigned char face_driver_num[20]={0};
	unsigned char got_net_status,ui_face_ret;
	unsigned char *pic_name;
//	ui_base_parameter_t* base_parameter;


	len_picname = (len - 19 - 1);//-driver_num(19) -face ret(1)
	printf("len_picname : %d \n\r",len_picname);
	//got pic path
	if((buf ==NULL)||(len_picname <= 0))
	{
		printf(" wget_post_pic_func use error!!!\n\r");
		return 0;
	}
	pic_name = malloc(len_picname);
	memset(pic_name,0,len_picname);
	//prase data
	ui_face_ret = *buf;
	buf_offset++;
	memcpy(&face_driver_num,buf+buf_offset,19);//driver_num(19)
	buf_offset+= 19;//driver_num(19)
	strncpy(pic_name,buf+buf_offset,len_picname);
	printf("pic_name::%s\n",pic_name);

	//pic file data to base64
	no_pic = read_pic_file_to_base64_code(sendbuf,pic_name);
	free(pic_name);
	if(no_pic == 1) // 打开文件失败
	{
		printf("---no-open-pic----\n");
		return 0;
	}
#if 0
	//file OK
	base_parameter = get_ui_base_parameter();
	if(strlen((char*)base_parameter->terminal_id)<=6)//only copy all data
	{
		strncpy(temp_mtd_id_str,(char*)base_parameter->terminal_id,6);
	}
	else //only copy last 6 byte
	{
		mtd_str_offset = strlen((char*)base_parameter->terminal_id)-6;
		strncpy(temp_mtd_id_str,(char*)base_parameter->terminal_id+mtd_str_offset,6);
	}
#endif
	 sprintf(cmd,"%s%s%s%s%s%s%s%s%s%s%s",cmd_head,cmd_log_path,cmd_json_license,face_driver_num,cmd_img,sendbuf,cmd_mdtId,\
	 "180884",cmd_json_end,cmd_server_baseaddr,cmd_face);

	popen_fp = popen(cmd,"r");
	if(popen_fp < 0)
	{
		printf("----popen-fail----\n");
		return 0;
	}
	popen_fd = fileno(popen_fp);
		

	struct timeval timeout={0,500}; //select等待10秒

	while(1)
	{

	        FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化
	        FD_SET(popen_fd,&fds); //添加描述符
	        timeout.tv_sec=0;
	        timeout.tv_usec=500;//select函数会不断修改timeout的值，所以每次循环都应该>重新赋值[windows不受此影响]
	        maxfdp=popen_fd+1; //描述符最大值加1

		select_ret = select(maxfdp,&fds,&fds,NULL,&timeout);//select使用
			
	        switch(select_ret) 
		{
			case -1:
					printf("popen erreor\n");
					got_net_status = 0;
					key = 1;
					break; //select错误，退出程序
			case 0:
					printf("popen timeout pp\n");
					system("killall wget -9");
					got_net_status = 0;
					key = 1;
					break; 
			default:
				
					for(;;)
					{
					fread(tem_buf,1,sizeof(tem_buf),popen_fp);
					if(feof(popen_fp))
					break;
					}
					got_net_status = 1;
					printf("popen default\n");
					key = 1;
					break;

		}

		if(key == 1)
		{
				printf("key 1: %d :%d\n",got_net_status,ret_wget);
			if((0 != got_net_status)||(0 == ret_wget))
			{
				printf("key 2: %d:%d\n",got_net_status,ret_wget);
				prase_json_face_ret_data(got_net_status);
			}
				printf("key 3: %d :%d\n",got_net_status,ret_wget);
			break;
		}
	}
	 

	pclose(popen_fp);

	return got_net_status;
}

#if 1
typedef struct{
	unsigned char ret;							
	unsigned char num_driver[19];						
	unsigned char pic_path[128];
}__attribute__((packed)) ui_proto_test_data;

void main(void)
{

	ui_proto_test_data  data_test;
	//unsigned char pathq[] = "/mnt/photo.jpg1";
	unsigned char pathq[] = "/home/fleety/share/test/20111212.jpg";
	unsigned char numaa[19] = "20111212";
	int wget_ret = 0;
	data_test.ret = 1;
	

	memcpy(data_test.num_driver,numaa,sizeof(numaa));
	memcpy(data_test.pic_path,pathq,sizeof(pathq));
	printf("data_test.pic_path %s \n",data_test.pic_path);
	printf("data_test.num_driver %s \n",data_test.num_driver);
	wget_ret = wget_post_pic_func((unsigned char *)&data_test,sizeof(ui_proto_test_data),1);
	if(wget_ret == 0)
	{
		printf("2wewew----------2\n");
	wget_post_pic_func((unsigned char *)&data_test,sizeof(ui_proto_test_data),wget_ret);
	}
}

#endif

