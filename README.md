# Multi-Threaded Task Scheduler

This project implements a **multi-threaded task scheduler** using a **thread pool** and a **thread-safe queue**. Tasks are read from a file and scheduled based on their **arrival time** and **burst time**.

## Features
- Uses a **ThreadPool** to manage worker threads.
- Implements a **ThreadQueue** with a custom **Mutex** for thread synchronization.
- Reads tasks from a file and sorts them based on their **arrival time**.
- Simulates real-time execution by delaying task addition based on arrival time.
- Uses **condition variables** for efficient thread synchronization.

## File Format
The input file should contain tasks in the following format:
```
<arrival_time> <burst_time>
```
Example (`Task.txt`):
```
0 2
1 3
4 1
6 5
```

## Code Structure
### 1. **Mutex**
A simple **spinlock-based mutex** using `std::atomic` to avoid busy-waiting.

### 2. **ThreadQueue**
A **thread-safe queue** with a maximum size limit using `Mutex` and `condition_variable_any`.

### 3. **Task Structure**
```cpp
struct Task {
    int id;
    int burstTime;
    int arrivalTime;
};
```
Each task has an **ID**, **burst time** (execution duration), and **arrival time** (when it should be added to the queue).

### 4. **ThreadPool**
A **thread pool** that maintains worker threads, which continuously fetch and execute tasks from `ThreadQueue`.

### 5. **FileReader**
Reads tasks from the input file, sorts them by **arrival time**, and adds them to the queue at the appropriate moment.

## How to Run
1. **Compile the Code**:
```sh
g++ -o app main.cpp
```

2. **Run the Executable**:
```sh
./app
```

## Configuration
- **Worker Count**: Adjust the number of worker threads in `main()`:
  ```cpp
  int workerCount = 3;
  ```
- **Queue Size**: Set the maximum size of `ThreadQueue`:
  ```cpp
  int queueSize = 4;
  ```
- **Input File**: Change the task file name if needed:
  ```cpp
  string fileName = "Task.txt";
  ```
  Note : Input Test in Task file

## Example Output
```
Worker 0 Started Task 1 (Arrival Time 0s)
Worker 0 Finished Task 1 (Burst Time 2s)
Worker 1 Started Task 2 (Arrival Time 1s)
Worker 1 Finished Task 2 (Burst Time 3s)
...
```

## Notes
- Ensure the input file follows the correct format.
- Tasks are scheduled based on **arrival time**, so they may not be executed immediately after reading.
- The program will terminate when all tasks are completed.