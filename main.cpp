#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <unistd.h>

using namespace std;

bool HoldAndWait(bool* lock) {
    bool old_value = *lock;
    *lock = true;
    return old_value;
}

class Mutex {
private:
    bool locked;

public:
    Mutex() : locked(false) {}

    void lock() {
        while (HoldAndWait(&locked)) {
            // Waitting For True Locked
        }
    }

    void unlock() {
        locked = false;
    }
};

struct Task {
    int id;
    int start_time;
    int end_time;
    int execution_time;
};

class ThreadPool {
    private:
        queue<Task> task_queue;
        vector<Task> completed_tasks;
        vector<pthread_t> threads;
        
        Mutex queue_mutex;
        pthread_mutex_t cond_mutex;
        pthread_cond_t cond_var;

        bool stop;
        int global_time;
        int num_threads;

    public:
        ThreadPool(int num_threads) : stop(false), global_time(0), num_threads(num_threads) {
            pthread_mutex_init(&cond_mutex, nullptr);
            pthread_cond_init(&cond_var, nullptr);
        }

        ~ThreadPool() {
            pthread_mutex_destroy(&cond_mutex);
            pthread_cond_destroy(&cond_var);
        }

        void addTask(Task task) {
            queue_mutex.lock();
            task_queue.push(task);
            queue_mutex.unlock();

            pthread_cond_signal(&cond_var);
        }

        static void* workerStatic(void* arg) {
            ThreadPool* pool = static_cast<ThreadPool*>(arg);
            pool->worker();
            return nullptr;
        }

        void worker() {
            while (true) {
                pthread_mutex_lock(&cond_mutex);

                while (task_queue.empty() && !stop) {
                    pthread_cond_wait(&cond_var, &cond_mutex);
                }

                if (stop && task_queue.empty()) {
                    pthread_mutex_unlock(&cond_mutex);
                    break;
                }

                queue_mutex.lock();
                Task task = task_queue.front();
                task_queue.pop();
                queue_mutex.unlock();

                pthread_mutex_unlock(&cond_mutex);

                sleep(task.execution_time);

                queue_mutex.lock();
                task.start_time = global_time;
                global_time += task.execution_time;
                task.end_time = global_time;
                completed_tasks.push_back(task);
                queue_mutex.unlock();
            }
        }

        void startThreads() {
            for (int i = 0; i < num_threads; ++i) {
                pthread_t thread;
                pthread_create(&thread, nullptr, workerStatic, this);
                threads.push_back(thread);
            }
        }

        void stopThreads() {
            stop = true;
            pthread_cond_broadcast(&cond_var);

            for (auto& thread : threads) {
                pthread_join(thread, nullptr);
            }
        }

        void report() {
            cout << "Simulation Result\n";
            cout << "----------------\n";
            if (!completed_tasks.empty()) {
                cout << "Total Simulation Time: " << completed_tasks.back().end_time << " Seconds\n";
            } else {
                cout << "Total Simulation Time: 0 Seconds\n";
            }
            cout << "Number Of Completed Tasks: " << completed_tasks.size() << "\n";

            for (const auto& task : completed_tasks) {
                cout << "- Task " << task.id << ": Start=" << task.start_time << "s, End=" << task.end_time << "s\n";
            }
        }
};

int main() {
    vector<Task> tasks = {
        {0, 0, 0, 3},
        {1, 0, 0, 4},
        {2, 0, 0, 7},
        {3, 0, 0, 5},
        {4, 0, 0, 6}
    };

    ThreadPool pool(3);
    pool.startThreads();

    for (const auto& task : tasks) {
        pool.addTask(task);
    }

    pool.stopThreads();
    pool.report();

    return 0;
}