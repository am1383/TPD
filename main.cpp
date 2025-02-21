#include "SimulationEnvironment.h"

int main() {
    SimulationEnvironment sim;
    sim.loadConfig("config.txt");  
    sim.runSimulation();  

    return 0;
}