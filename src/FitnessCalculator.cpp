#include <iostream>
#include <thread>
#include <chrono>
#include "Simulator.h"
#include "Agent.h"
#include "Controller.h"
#include "linuxMac.h"
#include <chrono>

float fitness(Controller *controller) {    
    Simulator simulator;
    Agent agent;
    int x = 0;
    int cnt = 0 ;
    // Main game loop
    while (simulator.isRunning() && !simulator.finishedMap()) {
        //simulator.handleEvents(agent);
        controller->action(agent, simulator);
        simulator.update(agent);
        cnt++ ;
    }    
    if(simulator.finishedMap()) return 1000 ;
    
    return cnt ;
}
