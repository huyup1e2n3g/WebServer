#include "threadpool.h"

// 线程池的实现
// 构造函数
template<typename T>
threadpool<T>::threadpool(int thread_number, int max_request) :
    m_thread_number(thread_number), m_max_requests(max_request),
    m_stop(false), m_threads(NULL) {
    
    // 线程数量和最大请求量不能为0
    if ((thread_number <= 0) || (max_request <= 0)) {
        throw std::exception();
    }

    // 创建数组，初始化子线程数组
    m_threads = new pthread_t[m_thread_number]; 
    if (!m_threads) {   // 如果没有创建成功
        throw std::exception();
    }

    // 创建 thread_number 个子线程，并将它们设置为线程脱离
    for (int i = 0; i < thread_number; i++) {
        printf("Create the %dth thread.\n", i);
        
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

// 主线程操作请求队列，往队列中添加任务，保证线程同步
template<typename T>
bool threadpool<T>::append(T * request) {
    m_queuelocker.lock();   // 上锁，主线程添加请求队列，此时其他线程不能操作队列 
    if (m_workqueue.size() > m_max_requests) {
        m_queuelocker.unlock(); // 如果超出最大量就解锁
        return false;           // 不能再添加
    }
    m_workqueue.push_back(request); // 可以添加，将request添加到队列的后面
    m_queuelocker.unlock();         // 解锁
    m_queuestat.post();           // 添加后，增加信号量，通知子线程来任务了
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

// 子线程操作请求队列（获取）,并工作
template<typename T>
void threadpool<T>::run() {
    while(!m_stop) {    // 当线程不停止
        m_queuestat.wait();     // 工作队列为空就等待挂起，工作队列不为空才能获取任务
        m_queuelocker.lock();   // 获取任务时，要使用互斥锁，保证对资源的独占式访问
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
        //调用任务的工作 逻辑函数
        request->process(); // 执行任务（工作），这里不用锁，并发执行
    }
}