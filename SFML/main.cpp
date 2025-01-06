#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib> 
#include <ctime>   

#include "Simulator.h"
#include "Agent.h"
#include "Controller.h"
#include "SelectionScreen.h"  // Include the selection screen header
#include "./NNlogic.cpp"

int main() {
    ControllerType selectedController = showSelectionScreen();
    if (selectedController == NONE) {
        std::cerr << "No controller selected, exiting.\n";
        return -1;
    }

    Simulator simulator;
    Bird agent;
    Controller* controller = nullptr;

    switch (selectedController) {
    case NN:
        controller = new MyController;  // Assuming NNController is defined
        break;
    case GP:
        controller = new MyController;  // Assuming GPController is defined
        break;
    case CGP:
        controller = NNlogic();  // Assuming CGPController is defined
        break;
    default:
        std::cerr << "Invalid controller selected, exiting.\n";
        return -1;
    }

    simulator.initialize(agent);

    // Main game loop
    while (simulator.isRunning()) {
        simulator.update(agent);
        controller->action(agent, simulator);
    }

    delete controller;
    return 0;
}