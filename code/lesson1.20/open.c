/*
// 头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// open 函数的声明
// 打开一个已经存在的文件
int open(const char *pathname, int flags);
		参数：
				- pathname：要打开的文件路径
				- flags：对文件的操作权限设置还有其他设置
						- O_RDONLY：只读
						- O_WRONLY：只写
						- O_RDWR：可读可写
						这三个设置是互斥的
		返回值：返回一个新的文件描述符，如果调用失败，返回 -1 ，系统会把错误号赋值给errno
		errno：属于Linux系统函数库，库里面的一个全局变量，记录的是最近的错误号
		perror：查看具体的错误

#include <stdio.h>
void perror(const char *s); 
		作用：打印 errno 对应的错误描述
		s 参数：用户描述，比如 hello，最终输出的内容是 hello:xxx(实际的错误描述)

#include <unistd.h>
int close(int fd);

// 创建一个新的文件
int open(const char *pathname, int flags, mode_t mode);
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // 打开一个文件
    int fd = open("a.txt", O_RDONLY);
    if(fd == -1) {
        perror("open");
    }
    // 关闭
    close(fd);
    return 0;
}