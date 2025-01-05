#include <iostream>
#include <thread>
#include <chrono>
#include "Simulator.h"
#include "Agent.h"
#include "Controller.h"
#include "CGP.h"
#include "Individual.h"

#define MAP "map1.txt"
#define MAP_NUM 1

int main() {

    Individual ind;
    CGP cgp(MAP, MAP_NUM);
    int choice = 0;

    //std::cout << "Generate/Load (0/1): ";
    //std::cin >> choice;

    //if (choice == 0) {
        ind = cgp.runCGP(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS);
    //}
    //else if (choice == 1) {
    //    std::ifstream inFile("CGP_best.txt");
    //    if (inFile.is_open()) {
    //        ind = Individual::deserialize(inFile);
    //        inFile.close();
    //        std::cout << "Object read from text file." << std::endl;
    //    }
    //}

    Simulator simulator(MAP);
    simulator.cgp = cgp;
    Agent agent;
    //Controller* controller = new Controller;  // base controller: takes no action
    CGPController* controller = new CGPController;  // plug in your controller here

    // Main game loop
    while (simulator.isRunning()) {
        if (simulator.handleEvents(agent) == false)
            controller->action(agent, simulator, ind);
        simulator.update(agent);
        simulator.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(60)); // Control game speed
    }

    std::cout << "Press q to exit...\n";
    while (simulator.isScoreScreenActive()) {
        simulator.handleScoreEvents();
    }

    return 0;
}
