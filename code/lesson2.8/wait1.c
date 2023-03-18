#include <sys/types.h>  // pid_t wait()
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>     // fork() 
#include <stdlib.h>

int main() {

    // 有一个父进程，创建5个子进程（兄弟）
    pid_t pid;

    // 创建5个子进程
    for(int i = 0; i < 5; i++) {
        pid = fork();
        // 如果是已经创建的子进程，就不继续创建子进程（孙子进程）
        if(pid == 0) {
            break;
        }
    }

    if(pid > 0) {
        // 父进程
        while(1) {
            printf("parent, pid = %d\n", getpid());
            sleep(1);
        }
    } 
    else if (pid == 0){
        // 子进程
        printf("child, pid = %d\n", getpid());    
        
    }

    return 0; 
}