#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <algorithm>

using namespace std;

class Mutex {
private:
    atomic<bool> locked;

public:
    Mutex() : locked(false) {}

    void lock() {
        while (locked.exchange(true, memory_order_acquire)) {
            this_thread::yield();
        }
    }

    void unlock() {
        locked.store(false, memory_order_release);
    }
};

template <typename T>
class ThreadQueue {
private:
    queue<T> queue;
    Mutex mutex;
    size_t maxSize;

public:
    ThreadQueue(size_t size) : maxSize(size) {}

    void Push(const T& item) {
        while (true) {
            mutex.lock();
            if (queue.size() < maxSize) {
                queue.push(item);
                mutex.unlock();
                return;
            }
            mutex.unlock();
            this_thread::yield();
        }
    }

    T Pop() {
        while (true) {
            mutex.lock();
            if (!queue.empty()) {
                T item = queue.front();
                queue.pop();
                mutex.unlock();
                return item;
            }
            mutex.unlock();
            this_thread::yield();
        }
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
    atomic<int> activeWorkers;
    atomic<bool> coutLock{false};

    void SafePrint(const string& message) {
        while (coutLock.exchange(true, memory_order_acquire)) {
            this_thread::yield();
        }
        cout << message << endl;
        coutLock.store(false, memory_order_release);
    }

    void Worker(int id) {
        while (!stop || !taskQueue.IsEmpty()) {
            if (!taskQueue.IsEmpty()) {
                Task task = taskQueue.Pop();
                SafePrint("Worker " + to_string(id) + " Started Task " + to_string(task.id) +
                          " <Arrival Time " + to_string(task.arrivalTime) + "s>");

                this_thread::sleep_for(chrono::seconds(task.burstTime));

                SafePrint("Worker " + to_string(id) + " Finished Task " + to_string(task.id) +
                          " <Execution Time " + to_string(task.burstTime) + "s>");
            } else {
                this_thread::yield();
            }
        }
        activeWorkers.fetch_sub(1, memory_order_relaxed);
    }

public:
    ThreadPool(int workerCount, int queue_size)
        : taskQueue(queue_size), stop(false), activeWorkers(workerCount) {
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
        SafePrint("Total workers used: " + to_string(workers.size()));
    }

    void AddTask(const Task& task) {
        taskQueue.Push(task);
    }
};

void FileReader(ThreadPool& pool, const string& fileName) {
    ifstream file(fileName);

    if (!file) {
        cerr << "File Cannot Be Open! " << fileName << "\n";
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
    int queueSize = 4;
    string fileName = "Task.txt";

    ThreadPool pool(workerCount, queueSize);
    FileReader(pool, fileName);

    return 0;
}
