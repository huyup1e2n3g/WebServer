#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <list>
#include "locker.h"
#include <exception>
#include <cstdio>

// 线程池类
// 定义成模板类是为了代码的复用，模板参数 T 是任务类
// 注意！！！模板定义声明最好在一个文件，不然可能会报错，所以 threadpool 不要分开写
template<typename T>
class threadpool {
public:
    threadpool(int thread_number = 8, int max_request = 10000);
    ~threadpool();
    bool append(T* request);    // 往工作队列添加任务
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


// 线程池的实现
// 构造函数
template<typename T>
threadpool<T>::threadpool(int thread_number, int max_request) :
    m_thread_number(thread_number), m_max_requests(max_request),
    m_stop(false), m_threads(NULL) {
    
    if ((thread_number <= 0) || (max_request <= 0)) {
        throw std::exception();
    }

    // 创建数组
    m_threads = new pthread_t[m_thread_number]; 
    if (!m_threads) {   // 如果没有创建成功
        throw std::exception();
    }

    // 创建 thread_number 个线程，并将它们设置为线程脱离
    for (int i = 0; i < thread_number; i++) {
        printf("Create the %dth thread.\n", i + 1);
        
        // 创建线程
        if (pthread_create(m_threads + i, NULL, worker, this) != 0) {   // 将 this 作为参数传递到 workder函数中
            delete[] m_threads;
            throw std::exception();
        }

        // 设置线程分离（自动销毁）
        if (pthread_detach(m_threads[i])) {
            delete[] m_threads;
            throw std::exception(); 
        }
    }
}

// 析构函数
template<typename T>
threadpool<T>::~threadpool() {
    delete[] m_threads;
    m_stop = true;  // 停止线程。默认为false，表示不停止线程。
}

// 往队列中添加任务，保证线程同步
template<typename T>
bool threadpool<T>::append(T * request) {
    m_queuelocker.lock();   // 上锁
    if (m_workqueue.size() > m_max_requests) {
        m_queuelocker.unlock(); // 如果超出最大量就解锁
        return false;           // 不能再添加
    }
    m_workqueue.push_back(request); // 可以添加，将request添加到队列的后面
    m_queuelocker.unlock();         // 解锁
    m_queuestat.post();           // 添加后，增加信号量
    return true;
}

template<typename T>
void* threadpool<T>::worker(void * arg) {
    // 将 this 作为参数传递到 workder函数中，就可以拿到对象了
    threadpool * pool = (threadpool *) arg;
    // 当一个线程创建出来以后，执行run()，让它跑起来
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run() {
    while(!m_stop) {    // 当线程不停止
        m_queuestat.wait();
        m_queuelocker.lock();
        if (m_workqueue.empty()) {
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();   // 获取任务
        m_workqueue.pop_front();
        m_queuelocker.unlock();

        if (!request) { // 如果没有获取任务
            continue;
        }

        request->process(); // 运行任务
    }
}

#endif