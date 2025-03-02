# **Thread Pool Simulator**  

## **Introduction**  
This project is a **Thread Pool Simulator** implemented in **C++** using `pthread`. It simulates the execution of multiple tasks by a fixed number of worker threads. Each task has a specified execution time, and the system reports the total execution time and task completion details.  

## **Features**  
- Implements a **thread pool** with **mutex** and **conditional variables** for synchronization.  
- Uses **a queue** to store tasks before execution.  
- **Parallel execution** of tasks using multiple worker threads.  
- **Reports** the total execution time and individual task completion details.  

## **Requirements**  
To compile and run the program, you need:  
- **C++ compiler** (`g++`)  
- **POSIX Threads Library (`pthread`)**  

## **Compilation & Execution**  

### **1. Compilation**  
Use the following command to compile the program:  
```bash
g++ -o app main.cpp
```

### **2. Running the Program**  
Simply execute the compiled program:  
```bash
./app
```

## **Program Behavior**  
- The number of worker threads is set to **3** inside the code.  
- The tasks are predefined in the program with different execution times.  
- The program automatically starts threads, processes tasks, and generates the report.  

## **Predefined Tasks in the Code**  
```
Task 0: Execution Time = 3s
Task 1: Execution Time = 4s
Task 2: Execution Time = 7s
Task 3: Execution Time = 5s
Task 4: Execution Time = 6s
```

## **Example Output**  
After execution, the program prints a **report** like this:  
```
Simulation Result
----------------
Total Simulation Time: 25 Seconds
Number Of Completed Tasks: 5
- Task 0: Start=0s, End=3s
- Task 1: Start=3s, End=7s
- Task 2: Start=7s, End=14s
- Task 3: Start=14s, End=19s
- Task 4: Start=19s, End=25s
```

## **Files Included**  
- `thread_pool.cpp` → Source code  
- `README.md` → Documentation  