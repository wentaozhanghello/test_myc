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
1. mkdir()函数创建路径的权限是其mode参数和进程的mask作用的结果，具体是mode&~mask 
2. 一般情况，root用户的mask值为022，那么我们调用mkdir()创建一个0777权限的路径，创建出的路径权限是0755。（0777&（~022）） 
3. 不同的用户默认的默认的mask值可能不一样，比如我的测试环境，root用户默认的mask值是022，名为tt的普通用户默认的mask值是02 
4. 必须创建一个让所有用户拥有读写权限的路径时，需要将mask设置成0，这样创建的路径的权限则完全由mkdir()的mode参数决定。 
5. 在一个比较大的项目中，如果为了添加路径修改了mask的值，建议创建完路径后将mask值还原。 
6. 看到网上有人问mkdir()的mode参数设置成777为什么不行，对此需要注意的是，0777是八进制数字，如果非要用十进制设置mode参数的话，用511也是等效的，只是代码阅读起来就不那么直观了
*/