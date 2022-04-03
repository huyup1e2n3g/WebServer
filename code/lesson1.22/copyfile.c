#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    // 1. 通过 open 打开 english.txt 文件
    int srcfd = open("english.txt", O_RDONLY);  // 源文件的文件描述符
    if (srcfd == -1){
        perror("open");
        return -1;
    }
    // 2. 创建一个心的文件（拷贝文件）
    int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0664); // 目标文件的描述符
    if (destfd == -1){
        perror("open");
        return -1;
    }
    // 3. 频繁的读写操作 
    char buf[1024] = {0};
    int len = 0;
    while((len = read(srcfd, buf, sizeof(buf))) > 0) {
        write(destfd, buf, len);
    }


    // 4. 关闭文件
    close(destfd);
    close(srcfd);
    return 0;
}