#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>    // 文件的状态
#include <sys/mman.h>   //  内存映射
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "locker.h"
#include <sys/uio.h>

class http_conn {
public:
    static int m_epollfd;   // 所有的 socket 上的事件都被注册到同一个 epoll 上
    static int m_user_count; // 统计用户的数量
    static const int READ_BUFFER_SIZE = 2048;   // 读缓冲区的大小
    static const int WRITE_BUFFER_SIZE = 1024;  // 写缓冲区的大小

public: // 状态（有限状态机）
    // HTTP 请求方法，但我们只支持GET
    enum METHOD {GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT};

    /* 
        解析客户端请求时，主状态机的状态
        CHECK_STATE_REQUESTLINE: 当前正在分析请求行
        CHECK_STATE_HEADER: 当前正在分析头部字段
        CHECK_STATE_CONTENT: 当前正在解析请求体 
    */
    enum CHECK_STATE {CHECK_STATE_REQUESTLINE, CHECK_STATE_HEADER, CHECK_STATE_CONTENT};

    // 从状态机的三种可能状态，即行的读取状态，分别表示
    // 1.读取到一个完整的行 2.行出错 3.行数据尚且不完整
    enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN };

    /*
        服务器处理HTTP请求的可能结果，报文解析的结果
        NO_REQUEST          :   请求不完整，需要继续读取客户数据
        GET_REQUEST         :   表示获得了一个完成的客户请求
        BAD_REQUEST         :   表示客户请求语法错误
        NO_RESOURCE         :   表示服务器没有资源
        FORBIDDEN_REQUEST   :   表示客户对资源没有足够的访问权限
        FILE_REQUEST        :   文件请求,获取文件成功
        INTERNAL_ERROR      :   表示服务器内部错误
        CLOSED_CONNECTION   :   表示客户端已经关闭连接了
    */
   enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION};


public:
    http_conn(){}
    ~http_conn(){}
    
public:
    void process(); // 处理客户端的请求
    void init(int sockfd, const sockaddr_in & addr);    // 初始化新接收的连接
    void close_conn();  // 关闭连接
    bool read();    // 非阻塞的读
    bool write();   // 非阻塞的写

private:

    void init();    // 初始化连接其余的信息

    HTTP_CODE process_read();   // 解析 HTTP 请求
    HTTP_CODE parse_request_line(char * text);     // 解析请求首行
    HTTP_CODE parse_headers(char * text);  // 解析请求头
    HTTP_CODE parse_content(char * text);  // 解析请求体

    LINE_STATUS parse_line();   // 解析行

    char * get_line() { return m_read_buf + m_start_line; } // 获取一行数据

    HTTP_CODE do_request();

private:
    int m_checked_index;    // 当前正在分析的字符在读缓冲区的位置
    int m_start_line;       // 当前正在解析的行的起始位置

    char * m_url;   // 请求目标文件的文件名
    char * m_version;   // 协议版本，只支持 HTTP1.1
    METHOD m_method;    // 请求方法
    char * m_host;      // 主机名
    bool m_linger;      // 判断 HTTP 请求是否要保持连接

    int m_content_length;

    CHECK_STATE m_check_state;  // 主状态机目前所处的状态

private:
    int m_sockfd;   // 该http连接的socket
    sockaddr_in m_address;  // 通信的socket地址
    char m_read_buf[READ_BUFFER_SIZE];  // 读缓冲区, m_read_buf 为头索引
    int m_read_idx; // 标识读缓冲区中已经读入的客户端数据的最后一个字节的下一个位置



};


#endif