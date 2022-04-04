#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
int main(){
    // 创建一个 aaa 目录
    int ret = mkdir("aaa", 0777);
    if(ret == -1){
        perror("mkdir");
        return -1;
    }
    return 0;
}