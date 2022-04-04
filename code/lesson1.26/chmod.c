#include <sys/stat.h>
#include <stdio.h>
int main(){
    // 修改文件权限
    int ret = chmod("a.txt", 0775);
    if(ret == -1){
        perror("chmod");
        return -1;
    }
    return 0;
}