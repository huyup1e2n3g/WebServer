#include <unistd.h>
#include <stdio.h>

int main() {
    // 判断文件是否存在
    int ret = access("a.txt", F_OK);
    if(ret == -1){
        perror("access");
    }
    printf("文件存在\n");
    return 0;
}