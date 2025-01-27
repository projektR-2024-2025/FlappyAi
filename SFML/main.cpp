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
#include "cgp_nikla/mainFunctionsHeader.h"
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

    menu(window);

    Parameters::simulationOnly = (Parameters::action == BEST) ? false : true;

    Controller* controller = nullptr;
    //CGP cgp(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS);


    switch (Parameters::ctrl) {
    case NN:
        controller = NNlogic();
        break;
    case GP1:
        controller = GPMain(Parameters::action);
        break;
    case CGP1:
        controller = CGP1::CGPMain(Parameters::action);   // Assuming CGPController is defined
        break;
    case CGP2:
        controller = new CGPController2(runCgp(Parameters::action), Parameters::action);
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
    

    cout << "main done\n";
    delete controller;
    return 0;
}