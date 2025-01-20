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

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Flappy AI");

    ControllerType selectedController = showSelectionScreen(window);
    if (selectedController == NONE) {
        std::cerr << "No controller selected, exiting.\n";
        return -1;
    }
    ActionType selectedAction = showActionScreen(window);

    Parameters::simulationOnly = selectedAction == BEST ? false : true;
    Parameters::randomPipes = false;

    Controller* controller = nullptr;
    CGP cgp(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS);

    switch (selectedController) {
    case NN:
        controller = NNlogic();
        break;
    case GP:
        controller = new Controller;
        break;
    case CGP1:
        controller = cgp.CGPMain(selectedAction);  // Assuming CGPController is defined
        break;
    default:
        std::cerr << "Invalid controller selected, exiting.\n";
        return -1;
    }

    Parameters::simulationOnly = false;
    Parameters::randomPipes = false;

    Simulator simulator(&window);
    Bird agent;
    simulator.initialize(agent);

    // Main game loop
    while (simulator.isRunning()) {
        simulator.update(agent);
        controller->action(agent, simulator);
    }

    delete controller;
    return 0;
}