#include <stdio.h>
int main(){
    // 将 aaa 目录改名为 bbb
    int ret = rename("aaa", "bbb");
    if(ret == -1){
        perror("rename");
        return -1;
    }
    return 0;   
}