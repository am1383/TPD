#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Task.h"

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<Task> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

public:
    ThreadPool(int numThreads);
    void enqueueTask(Task task);
    void workerThread();
    ~ThreadPool();
};

ThreadPool::ThreadPool(int numThreads) : stop(false) {
    for (int i = 0; i < numThreads; i++) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

void ThreadPool::enqueueTask(Task task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskQueue.push(task);
    }
    condition.notify_one(); 
}

void ThreadPool::workerThread() {
    while (true) {
        Task task(0, 0);
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]() { return !taskQueue.empty() || stop; });

            if (stop && taskQueue.empty())
                return;

            task = taskQueue.front();
            taskQueue.pop();
        }
        task.execute();
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

#endif