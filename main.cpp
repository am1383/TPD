#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <algorithm>

using namespace std;

class Mutex {
private:
    atomic<bool> locked;

public:
    Mutex() : locked(false) {
        //
    }

    void lock() {  
        while (locked.exchange(true, memory_order_acquire)) {
            this_thread::yield();  // Spinlock
        }
    }

    void unlock() {  
        locked.store(false, memory_order_release);
    }
};

template <typename T>
class ThreadQueue {
private:
    std::queue<T> queue;
    Mutex mutex;
    condition_variable_any cvNotEmpty;
    condition_variable_any cvNotFull;
    size_t maxSize;

public:
    ThreadQueue(size_t size) : maxSize(size) {}

    void Push(const T& item) {
        mutex.lock();

        while (queue.size() >= maxSize) {
            //
        }

        queue.push(item);
        cvNotEmpty.notify_one();

        mutex.unlock();
    }

    T Pop() {
        mutex.lock();

        while (queue.empty()) {
            cvNotEmpty.wait(mutex);
        }

        T item = queue.front();

        queue.pop();
        cvNotFull.notify_one();
        mutex.unlock();

        return item;
    }

    bool IsEmpty() {
        mutex.lock();
        bool empty = queue.empty();
        mutex.unlock();

        return empty;
    }
};

struct Task {
    int id;
    int burstTime;
    int arrivalTime;
};

class ThreadPool {
private:
    ThreadQueue<Task> taskQueue;
    vector<thread> workers;
    atomic<bool> stop;

    void Worker(int id) {
        while (!stop) {
            Task task;
            try {
                task = taskQueue.Pop();
            } catch (const runtime_error&) {
                continue;
            }

            cout << "Worker " << id << " Started Task "  << task.id
                      << " <Arrival Time " << task.arrivalTime << "s>\n";

            cout << "Worker " << id << " Finished Task " << task.id
                      << " <Excution Time "   << task.burstTime << "s>\n";
        }
        exit(1);
    }

public:
    ThreadPool(int workerCount, int queue_size)
        : taskQueue(queue_size), stop(false) {
        for (int i = 0; i < workerCount; ++i) {
            workers.emplace_back(&ThreadPool::Worker, this, i);
        } 
    }

    ~ThreadPool() {
        stop = true;

        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void AddTask(const Task& task) {
        taskQueue.Push(task);
    }
};

void FileReader(ThreadPool& pool, const string& fileName) {
    ifstream file(fileName);

    if (!file) {
        cerr << "File Cannot Be Open!" << fileName << "\n";
        return;
    }

    vector<Task> tasks;
    string line;
    int counterId = 1;

    while (getline(file, line)) {
        istringstream iss(line);
        int arrival, burst;

        if (!(iss >> arrival >> burst)) {
            continue;
        }

        tasks.push_back({counterId++, burst, arrival});
    }

    file.close();

    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    auto startTime = chrono::steady_clock::now();

    for (const auto& task : tasks) {
        this_thread::sleep_until(startTime + chrono::seconds(task.arrivalTime));
        pool.AddTask(task);
    }
}

int main() {
    int workerCount = 3;      
    int qeueSize    = 4;       

    string fileName = "Task.txt";  

    ThreadPool pool(workerCount, qeueSize);
    FileReader(pool, fileName);

    return 0;
}