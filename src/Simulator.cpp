#include "Simulator.h"
#include <iostream>
#include <conio.h> // For _kbhit() and _getch()

Simulator::Simulator() : running(true), birdPosition(10), groundLevel(20) {}

bool Simulator::isRunning() const {
    return running;
}

void Simulator::handleEvents(Agent& agent) {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 'q') {
            running = false;
        }
        else {
            agent.handleInput(ch);
        }
    }
}

void Simulator::update(Agent& agent) {
    agent.update(birdPosition);
    // Update game elements like bird position, collision detection, etc.
    if (birdPosition >= groundLevel) {
        running = false; // Game over if bird hits the ground
    }
}

void Simulator::render() {
    system("cls"); // Clear the console
    for (int i = 0; i < groundLevel; ++i) {
        if (i == birdPosition) {
            std::cout << "O\n"; // Bird
        }
        else {
            std::cout << "\n";
        }
    }
    std::cout << "====================\n"; // Ground
}