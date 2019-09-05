#include<sys/stat.h>
#include<iostream>
using namespace std;

int main(){
    char* filepath = "/home/fleety/share/test/c++/hello/";
    mode_t mode = umask(0);
    mkdir(filepath,0777);
    //do something
    umask(mode);
    //do something
    return 0;
}
/*
1. mkdir()��������·����Ȩ������mode�����ͽ��̵�mask���õĽ����������mode&~mask 
2. һ�������root�û���maskֵΪ022����ô���ǵ���mkdir()����һ��0777Ȩ�޵�·������������·��Ȩ����0755����0777&��~022���� 
3. ��ͬ���û�Ĭ�ϵ�Ĭ�ϵ�maskֵ���ܲ�һ���������ҵĲ��Ի�����root�û�Ĭ�ϵ�maskֵ��022����Ϊtt����ͨ�û�Ĭ�ϵ�maskֵ��02 
4. ���봴��һ���������û�ӵ�ж�дȨ�޵�·��ʱ����Ҫ��mask���ó�0������������·����Ȩ������ȫ��mkdir()��mode���������� 
5. ��һ���Ƚϴ����Ŀ�У����Ϊ�����·���޸���mask��ֵ�����鴴����·����maskֵ��ԭ�� 
6. ��������������mkdir()��mode�������ó�777Ϊʲô���У��Դ���Ҫע����ǣ�0777�ǰ˽������֣������Ҫ��ʮ��������mode�����Ļ�����511Ҳ�ǵ�Ч�ģ�ֻ�Ǵ����Ķ������Ͳ���ôֱ����
*/