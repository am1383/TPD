# Thread Pool Task Scheduler

## Overview
This C++ program implements a simple **Thread Pool Task Scheduler** that reads tasks from a file, sorts them by arrival time, and assigns them to worker threads for execution. It features a custom **mutex**, a **thread-safe queue**, and an **efficient thread pool** for task management.

## Features
- **Custom Mutex Implementation**: Uses an atomic-based spinlock.
- **Thread-Safe Queue**: Implements a thread-safe queue with a fixed max size.
- **Thread Pool**: Manages multiple worker threads for executing tasks.
- **Task Scheduling**: Reads tasks from a file, sorts them by arrival time, and assigns them to available workers.
- **Safe Console Output**: Prevents race conditions during logging.

## Compilation & Execution
### Prerequisites
- C++11 or later (for `std::thread`, `std::atomic`, and `std::chrono`).
- A C++ compiler (GCC, Clang, or MSVC).

### Compilation
Use the following command to compile the program:
```sh
 g++ -std=c++11 -o task_scheduler main.cpp
```

### Execution
```sh
 ./task_scheduler
```
Ensure the **task file** (`Task.txt`) is present in the same directory.

## Task File Format
The input file (`Task.txt`) should contain **tasks**, each defined by an arrival time and a burst time (execution duration), separated by a space. Example:
```txt
0 3  # Task arrives at 0s, runs for 3s
2 2  # Task arrives at 2s, runs for 2s
4 1  # Task arrives at 4s, runs for 1s
```

## Code Structure
### 1. **Mutex** (`Mutex` class)
A spinlock mutex using `std::atomic<bool>` to ensure safe access to shared resources.

### 2. **Thread-Safe Queue** (`ThreadQueue<T>` class)
A bounded queue allowing thread-safe **push** and **pop** operations with busy-waiting.

### 3. **Task Representation** (`Task` struct)
Holds task metadata:
- `id` - Unique task identifier
- `arrivalTime` - Time when the task should be scheduled
- `burstTime` - Execution duration

### 4. **Thread Pool** (`ThreadPool` class)
- Manages a fixed number of worker threads.
- Uses a shared queue for task distribution.
- Ensures proper thread synchronization and graceful shutdown.

### 5. **File Reader** (`FileReader` function)
- Reads tasks from a file.
- Sorts tasks by arrival time.
- Pushes them to the thread pool at the correct time.

### 6. **Main Function**
- Initializes the thread pool.
- Reads tasks from the file.
- Starts processing tasks asynchronously.

## Example Output
```
Worker 0 Started Task 1 <Arrival Time 0s>
Worker 0 Finished Task 1 <Execution Time 3s>
Worker 1 Started Task 2 <Arrival Time 2s>
Worker 1 Finished Task 2 <Execution Time 2s>
Worker 2 Started Task 3 <Arrival Time 4s>
Worker 2 Finished Task 3 <Execution Time 1s>
Total workers used: 3
```

## License
This project is released under the MIT License.