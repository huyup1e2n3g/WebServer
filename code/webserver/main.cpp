#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "locker.h"
#include "threadpool.h"
#include <signal.h>
#include "http_conn.h"
#include <string.h>     // memset

#define MAX_FD 65535    // 最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000  // 一次监听的最大的事件数量

// 信号处理
// 添加信号捕捉
void addsig(int sig, void(handler)(int)) {
    struct sigaction sa;    // 注册信号的参数
    memset(&sa, '\0', sizeof(sa));  // 清空
    sa.sa_handler = handler;    
    sigfillset(&sa.sa_mask);    // 设置临时阻塞信号集
    sigaction(sig, &sa, NULL);  // 注册信号
}

// 添加文件描述符到 epoll 中
extern void addfd(int epollfd, int fd, bool one_shot);  // 声明为外部的函数
// 从 epoll 中删除文件描述符
extern void removefd(int epollfd, int fd);
// 修改文件描述符
extern void modfd(int epollfd, int fd, int ev);

int main(int argc, char* argv[]) {
    
    // 传递端口号，须判断端口号是否有效
    if (argc <= 1) {
        printf("按照如下格式运行: %s port_number\n", basename(argv[0]));
        exit(-1);
    }

    // 获取端口号
    int port = atoi(argv[1]);   // 字符串转换为整数

    // 对 SIGPIE 信号进行处理
    addsig(SIGPIPE, SIG_IGN);   // 当捕捉到 SIGPIE 信号时，忽略该信号，什么都不做，不会结束程序

    // 创建线程池，初始化线程池
    threadpool<http_conn> * pool = NULL;    //  任务类是 连接http 类  
    try {
        pool = new threadpool<http_conn>;
    } catch(...) {
        exit(-1);
    }

    // 创建一个数组，用于保存所有的客户端信息
    http_conn * users = new http_conn[MAX_FD];
    
    // 网络通信
    // 创建监听的套接字
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);

    // 设置端口复用
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)); 

    // 绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    bind(listenfd, (struct sockaddr*)&address, sizeof(address));

    // 监听
    listen(listenfd, 5);

    // IO多路复用，使用 epoll 同时检测多个客户端
    // 创建 epoll 对象、事件数组，添加监听的文件描述符
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd =  epoll_create(5);

    // 将监听的文件描述符添加到 epoll 对象中
    addfd(epollfd, listenfd, false);
    http_conn::m_epollfd = epollfd;

    // 主线程循环检测事件的发生
    while(true) {
        int num = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1); // 检测到的事件数
        if ((num < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }

        // 循环遍历事件数组
        for (int i = 0; i < num; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) {
                // 有客户端连接进来
                struct sockaddr_in client_address;
                socklen_t client_addrlen = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlen);
                
                if (http_conn::m_user_count >= MAX_FD) {
                    // 目前的连接数满了
                    // 给客户端写一个信息：服务器内部正忙
                    close(connfd);
                    continue;
                }

                // 将新的客户的数据初始化，放到数组中
                users[connfd].init(connfd, client_address);
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                // 对方异常断开或者错误的事件
                // 关闭连接
                users[sockfd].close_conn();
            }
            else if (events[i].events & EPOLLIN) {
                // 检测到读事件
                if (users[sockfd].read()) {
                    // 一次性把数据读出来
                    pool->append(users + sockfd);
                }
                else {
                    users[sockfd].close_conn();
                }
            }
            else if (events[i].events & EPOLLOUT) {
                // 检测到写事件
                if (!users[sockfd].write()) {
                    // 一次性写完所有的数据
                    users[sockfd].close_conn();
                }
            }
        }
    }
    
    close(epollfd);
    close(listenfd);
    delete[] users;
    delete pool;
    return 0;
} 