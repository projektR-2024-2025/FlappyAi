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
#include "mainFunctionsHeader.h"
#include "CGP.h"
#include "gp_tonka/GA.h"
#include "ConfigParser.h"

int main(int argc, char** argv) {

    if(argc >= 2)
        parseConfig(argv[1]);
    else
        parseConfig("config.conf");

    sf::RenderWindow window(sf::VideoMode(Parameters::WINDOW_WIDTH, Parameters::WINDOW_HEIGHT), "Flappy AI");
    window.setFramerateLimit(Parameters::FRAME_RATE);

    ControllerType selectedController = showSelectionScreen(window);
    if (selectedController == NONE) {
        std::cerr << "No controller selected, exiting.\n";
        return -1;
    }



    ActionType selectedAction = showActionScreen(window);

    Parameters::simulationOnly = (selectedAction == BEST) ? false : true;

    Controller* controller = nullptr;
    CGP cgp(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS);


    switch (selectedController) {
    case NN:
        controller = NNlogic();
        break;
    case GP:
        controller = GPMain(selectedAction);
        break;
    case CGP1:
        controller = cgp.CGPMain(selectedAction);  // Assuming CGPController is defined
        break;
    case CGP2:
        controller = new CGPController2(runCgp(Parameters::simulationOnly), Parameters::simulationOnly);
        break;

    default:
        std::cerr << "Invalid controller selected, exiting.\n";
        return -1;
    }

    Parameters::simulationOnly = false;

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