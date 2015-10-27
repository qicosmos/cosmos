#pragma once
#include<list>
#include<thread>
#include<functional>
#include<memory>
#include <atomic>
#include "SyncQueue.hpp"

const int MaxTaskCount = 100;
class ThreadPool
{
public:
    using Task = std::function<void()>;
    ThreadPool(int numThreads = std::thread::hardware_concurrency()) : m_queue(MaxTaskCount)
    {
        Start(numThreads);
    }

    ~ThreadPool(void)
    {
        //如果没有停止时则主动停止线程池
        Stop();
    }

    void Stop()
    {
        std::call_once(m_flag, [this]{StopThreadGroup(); }); //保证多线程情况下只调用一次StopThreadGroup
    }

    void AddTask(Task&&task)
    {
        m_queue.Put(std::forward<Task>(task));
    }

    void AddTask(const Task& task)
    {
        m_queue.Put(task);
    }

private:
    void Start(int numThreads)
    {
        m_running = true;
        //创建线程组
        for (int i = 0; i <numThreads; ++i)
        {
            m_threadgroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
        }
    }    

    void RunInThread()
    {
        while (m_running)
        {
            //取任务分别执行
            std::list<Task> list;
            m_queue.Take(list);

            for (auto& task : list)
            {
                if (!m_running)
                    return;

                task();
            }
        }
    }

    void StopThreadGroup()
    {
        m_queue.Stop(); //让同步队列中的线程停止
        m_running = false; //置为false，让内部线程跳出循环并退出

        for (auto thread : m_threadgroup) //等待线程结束
        {
            if (thread)
                thread->join();
        }
        m_threadgroup.clear();
    }

    std::list<std::shared_ptr<std::thread>> m_threadgroup; //处理任务的线程组
    SyncQueue<Task> m_queue; //同步队列     
    atomic_bool m_running; //是否停止的标志
    std::once_flag m_flag;
};
