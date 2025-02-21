# Thread Pool Simulation in C++

## Overview
This project implements a **Thread Pool** in C++ to efficiently manage concurrent task execution. It includes:
- **ThreadPool Manager**: Creates and manages worker threads.
- **Worker Threads**: Execute tasks from a **task queue**.
- **Task Queue**: A thread-safe queue that holds pending tasks.
- **Simulation Environment**: Reads input configurations, runs the simulation, and generates reports.

## Features
- Multi-threaded execution using **std::thread**, **mutex**, and **condition_variable**.
- Thread-safe **task queue**.
- Configurable thread pool with **dynamic task handling**.
- Simulation environment that **logs task execution** and generates a final report.

## Prerequisites
- **C++11 or later**
- A C++ compiler like `g++`

## Installation & Compilation
### **Step 1: Clone the Repository**
```bash
 git clone <repository_link>
 cd thread-pool-simulation
```
### **Step 2: Compile the Code**
```bash
g++ -std=c++11 main.cpp -o simulation
```

## Usage
### **Step 1: Create an Input Configuration File**
The input file (e.g., `config.txt`) should be formatted as follows:
```
3  # Number of worker threads
1 500  # Task ID 1, Execution time 500ms
2 700  # Task ID 2, Execution time 700ms
3 600  # Task ID 3, Execution time 600ms
4 800  # Task ID 4, Execution time 800ms
```

### **Step 2: Run the Simulation**
```bash
./simulation
```

### **Step 3: View the Output**
The program prints logs such as:
```
Task 1 completed in 500ms
Task 2 completed in 700ms
Task 3 completed in 600ms
Task 4 completed in 800ms
```

## Project Structure
```
├── Task.h                  # Defines a task with execution time
├── ThreadPool.h            # Implements the thread pool
├── SimulationEnvironment.h # Handles simulation logic
├── main.cpp                # Runs the simulation
├── config.txt              # Sample input file
└── README.md               # Project documentation
```

## Future Improvements
- Implement **dynamic thread scaling** based on workload.
- Use **std::future** for retrieving task execution results.
- Improve **logging** and reporting mechanisms.

## License
This project is licensed under the **MIT License**.

## Author
Developed by **Your Name**
