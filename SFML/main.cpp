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
#include "cgp_nikla/mainFunctionsHeader.h"
#include "./nn/NNlogic.h"
#include "cgp_andrija/CGP1.h"
#include "gp_tonka/GA.h"
#include "ConfigParser.h"

int main(int argc, char** argv) {

    if(argc >= 2)
        parseConfig(argv[1]);
    else
        parseConfig("config.conf");

    sf::RenderWindow window(sf::VideoMode(Parameters::WINDOW_WIDTH, Parameters::WINDOW_HEIGHT), "Flappy AI");
    window.setFramerateLimit(Parameters::FRAME_RATE);

    
    while (window.isOpen()) {
        if (menu(window) == -1)
            return 0;

        Parameters::simulationOnly = (Parameters::action == BEST) ? false : true;

        Controller* controller = nullptr;

        switch (Parameters::ctrl) {
        case NN:
            controller = NNMain(window);
            break;
        case GP1:
            controller = GPMain(window);
            break;
        case CGP1:
            controller = cgp_andrija::CGP1::CGPMain(window);
            break;
        case CGP2:
            controller = new CGPController2(runCgp(Parameters::action, window), Parameters::action);
            break;
        case MANUAL:
            controller = new Controller;
            break;
        default:
            std::cerr << "Invalid controller selected, exiting.\n";
            return -1;
        }

        Parameters::simulationOnly = false;
        Parameters::action = BEST;

        Simulator simulator(&window);
        Bird agent;
        simulator.initialize(agent);

        // Main game loop
        while (simulator.isRunning()) {
            simulator.update(agent);
            controller->action(agent, simulator);
        }

        delete controller;
    }

    return 0;
}