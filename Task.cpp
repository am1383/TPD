#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <thread>
#include <chrono>

class Task {

public:
    int task_id;
    int execution_time; 

    Task(int id, int exec_time) : task_id(id), execution_time(exec_time) {}

    void execute() {
        std::this_thread::sleep_for(std::chrono::milliseconds(execution_time));
        std::cout << "Task " << task_id << " Completed In " << execution_time << "ms\n";
    }
};

#endif