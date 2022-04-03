#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    // 打开文件
    int fd = open("hello.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    //拓展文件
    int ret = lseek(fd, 100, SEEK_END);   // 从文件末尾开始拓展 100 字节
    if (ret == -1) {
        perror("lseek");
        return -1;
    }

    // 写入一个空数据，否则不会文件拓展
    write(fd, " ", 1);

    // 关闭文件
    close(fd);
    return 0;
}