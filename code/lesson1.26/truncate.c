#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
int main(){
    // 扩大文件大小
    int ret = truncate("b.txt", 20);
    if(ret == -1){
        perror("truncate");
        return -1;
    }
    return 0;
}