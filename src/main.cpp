#include <iostream>
#include <thread>
#include <chrono>
#include "Simulator.h"
#include "Agent.h"
#include "Controller.h"
#include "CGP.h"
#include "Individual.h"

#define MAP "map2.txt"

int main() {

    Individual ind;
    int choice = 0;

    std::cout << "Generate/Load (0/1): ";
    std::cin >> choice;

    if (choice == 0) {
        CGP cgp(MAP);
        ind = cgp.runCGP(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS);
    }
    else if (choice == 1) {
        std::ifstream inFile("CGP_best.txt");
        if (inFile.is_open()) {
            ind = Individual::deserialize(inFile);
            inFile.close();
            std::cout << "Object read from text file." << std::endl;
        }
    }

    Simulator simulator(MAP);
    Agent agent;
    //Controller* controller = new Controller;  // base controller: takes no action
    MyController* controller = new MyController;  // plug in your controller here

    // Main game loop
    while (simulator.isRunning()) {
        if (simulator.handleEvents(agent) == false)
            controller->action(agent, simulator, ind);
        simulator.update(agent);
        simulator.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(60)); // Control game speed
    }

    //system("cls");
    std::cout << "Press q to exit...\n";
    while (simulator.isScoreScreenActive()) {
        simulator.handleScoreEvents();
    }

    return 0;
}
