#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

class Task {

public:
    int task_id;
    int execution_time; 

    Task(int id, int exec_time) : task_id(id), execution_time(exec_time) {}

    void execute() {
        this_thread::sleep_for(chrono::milliseconds(execution_time));
        cout << "Task " << task_id << " Completed In " << execution_time << "ms\n";
    }
};

#endif