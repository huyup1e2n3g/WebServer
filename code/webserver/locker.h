#ifndef LOCKER_H   
#define LOCKER_H

/*
    宏定义:
    #ifndef 如果没有定义 LOCKER_H 
    #define 定义 LOCKETR_H
    #endif  end结束  
*/

#include <pthread.h>    // 互斥锁
#include <exception>    // 异常
#include <semaphore.h>  // 信号量

// 线程同步机制封装类
// 互斥锁类
class locker {
public:
    locker() {  // 构造函数
        if (pthread_mutex_init(&m_mutex, NULL) != 0) {
            throw std::exception(); // 抛出异常对象
        }
    }

    ~locker() { // 析构函数
        pthread_mutex_destroy(&m_mutex);
    }
    
    bool lock() {   // 提供锁的方式
        return pthread_mutex_lock(&m_mutex) == 0;
    }

    bool unlock() {   // 提供解锁的方式
        return pthread_mutex_unlock(&m_mutex) == 0;
    }
    pthread_mutex_t * get() {   // 获取成员的互斥量
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;    // 互斥锁
};

// 条件变量类
class cond {
public:
    cond() {
        if (pthread_cond_init(&m_cond, NULL) != 0) {
            throw std::exception();
        }
    }

    ~cond() {
        pthread_cond_destroy(&m_cond);
    }

    bool wait(pthread_mutex_t * mutex) {
        return pthread_cond_wait(&m_cond, mutex) == 0;
    }

    bool timedwait(pthread_mutex_t * mutex, struct timespec t) {
        return pthread_cond_timedwait(&m_cond, mutex, &t) == 0;
    }

    bool signal() {
        return pthread_cond_signal(&m_cond) == 0;
    }

    bool broadcast() {  // 将所有的线程都唤醒
        return pthread_cond_broadcast(&m_cond) == 0;
    }

private:
    pthread_cond_t m_cond;
};

// 信号量类
class sem {
public:
    sem() {
        if (sem_init(&m_sem, 0, 0) != 0) {
            throw std::exception();
        }
    }

    sem(int num) {  // 初始化值num
        if (sem_init(&m_sem, 0, num) != 0) {
            throw std::exception();
        }
    }

    ~sem() {
        sem_destroy(&m_sem);
    }

    bool wait() {   // 等待信号量
        return sem_wait(&m_sem) == 0;
    }

    bool post() {   // 增加信号量
        return sem_post(&m_sem) == 0;
    }

private:
    sem_t m_sem;
};


#endif