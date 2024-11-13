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
        std::this_thread::sleep_for(std::chrono::milliseconds(60)); // Control game speed
    }

    //system("cls");
    std::cout << "Press q to exit...\n";
    while (simulator.isScoreScreenActive()) {
        simulator.handleScoreEvents();
    }


    return 0;
}
