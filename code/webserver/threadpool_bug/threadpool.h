#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <list>
#include "locker.h"
#include <exception>
#include <cstdio>

// 线程池类
// 定义成模板类是为了代码的复用，模板参数 T 代表任务类
// 注意！！！模板定义声明最好在一个文件，不然可能会报错，所以 threadpool 不要分开写
template<typename T>
class threadpool {
public:
    threadpool(int thread_number = 8, int max_request = 10000);
    ~threadpool(); 
    bool append(T* request);    // 主线程往工作队列添加任务
private:
    static void* worker(void * arg);    // worker 必须是静态的函数，它不能访问当前对象非静态成员
    void run();    // 运行线程池，从工作队列中取数据，执行任务
private:
    // 线程的数量
    int m_thread_number;

    // 线程池数组（容器），大小为 m_thread_number
    pthread_t * m_threads;

    // 请求队列中最多允许的，等待处理的请求数量
    int m_max_requests;

    // 请求队列
    std::list<T*> m_workqueue;

    // 互斥锁
    locker m_queuelocker;

    // 信号量，用来判断是否有任务需要处理
    sem m_queuestat;

    // 是否结束线程
    bool m_stop;
};

#endif