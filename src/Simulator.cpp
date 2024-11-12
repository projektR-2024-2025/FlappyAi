#include "Simulator.h"
#include <iostream>
#include <conio.h> // For _kbhit() and _getch()

Simulator::Simulator() : running(true), birdPosition(10), groundLevel(20), scoreScreen(true) {}

bool Simulator::isRunning() const {
    return running;
}

bool Simulator::isScoreScreenActive() const {
    return scoreScreen;
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

// Handling events for score screen.
void Simulator::handleScoreEvents() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 'q') {
            std::cout << "The END!";
            scoreScreen = false;
        }
    }
}

void Simulator::update(Agent& agent) {
    agent.update(birdPosition);
    // Update game elements like bird position, collision detection, etc.
    if (birdPosition >= groundLevel || birdPosition <= 0) {
        running = false; // Game over if bird hits the ground or the ceiling
    }
}

void Simulator::render() {
    system("cls"); // Clear the console

    std::cout << "====================\n"; // Ceiling
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
