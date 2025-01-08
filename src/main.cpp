#include <iostream>
#include <thread>
#include <chrono>
#include "Simulator.h"
#include "Agent.h"
#include "Controller.h"
#include "linuxMac.h"


int main() {    
    Simulator simulator;
    Agent agent;
    //Controller* controller = new Controller;  // base controller: takes no action
    Controller* controller = new MyController;  // plug in your controller here

    setNonCanonicalMode(true); //linux/mac (za windows zakomentirajte ovu liniju)

    // Main game loop
    while (simulator.isRunning()) {
        simulator.handleEvents(agent);/*
        if (simulator.handleEvents(agent) == false)
            controller->action(agent, simulator);*/
        simulator.update(agent);
        simulator.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(60)); // Control game speed
    }

    //system("cls");
    std::cout << "Press q to exit...\n";
    while (simulator.isScoreScreenActive()) {
        simulator.handleScoreEvents();
    }

    setNonCanonicalMode(false); //linux/mac (za windows zakomentirajte ovu liniju)
    return 0;
}
