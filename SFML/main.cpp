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

int main() {

    ControllerType selectedController = showSelectionScreen();
    string answer;
    cout << "learning or running?(L/R)\n";
    cin >> answer;
    if (selectedController == NONE) {
        std::cerr << "No controller selected, exiting.\n";
        return -1;
    }


    //Entity bestEntityFromLastPop = runCgp().getBestEntity();

    Simulator simulator = Simulator();
    Bird agent = Bird();
    Controller* controller = nullptr;

    switch (selectedController) {
    case NN:
        *controller = NNlogic();
        break;
    case GP:
        controller = new Controller;
        break;
    case CGP:
        //controller = new CGPController(CGP::CGPMain());  // Assuming CGPController is defined
        controller = new CGPController2(runCgp(answer),answer);
        break;
    default:
        std::cerr << "Invalid controller selected, exiting.\n";
        return -1;
    }

    //cout << "simulator.initialize(agent);\n";
    simulator.initialize(agent);

    // Main game loop
    while (simulator.isRunning()) {
        //cout << "while (simulator.isRunning())\n";
        simulator.update(agent);
        controller->action(agent, simulator);
    }

    cout << "delete controller\n";
    delete controller;
    return 0;
}