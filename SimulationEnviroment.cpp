#ifndef SIMULATIONENVIRONMENT_H
#define SIMULATIONENVIRONMENT_H

#include <fstream>
#include <sstream>
#include <vector>
#include "ThreadPool.h"

using namespace std;

class SimulationEnvironment {

private:
    int numThreads;
    vector<Task> taskList;

public:
    void loadConfig(string filename);
    void runSimulation();
};

void SimulationEnvironment::loadConfig(string filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open file " << filename << "\n";
        return;
    }

    file >> numThreads;
    int task_id, exec_time;
    while (file >> task_id >> exec_time) {
        taskList.emplace_back(task_id, exec_time);
    }
}

void SimulationEnvironment::runSimulation() {
    ThreadPool pool(numThreads);

    for (Task &task : taskList) {
        pool.enqueueTask(task);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

#endif