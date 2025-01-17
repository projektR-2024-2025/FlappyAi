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
#include "NNlogic.h"
#include "CGP.h"

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
        *controller = NNlogic();
        break;
    case GP:
        controller = new Controller;
        break;
    case CGP:
        controller = new CGPController(CGP::CGPMain());  // Assuming CGPController is defined
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