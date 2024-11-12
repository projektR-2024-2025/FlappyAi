#include <iostream>
#include <thread>
#include <chrono>
#include "Simulator.h"
#include "Agent.h"

int main() {
    Simulator simulator;
    Agent agent;

    // Main game loop
    while (simulator.isRunning()) {
        simulator.handleEvents(agent);
        simulator.update(agent);
        simulator.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Control game speed
    }

    return 0;
}