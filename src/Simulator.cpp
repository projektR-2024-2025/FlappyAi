#include "Simulator.h"
#include <iostream>
#include <conio.h> // For _kbhit() and _getch()

Simulator::Simulator() : running(true), birdPosition(10), scoreScreen(true) {
    for (int i = 0; i < groundLevel; i++) {
        for (int j = 0; j < viewWidth; j++) {
            obstacleMap[i][j] = false;
        }
    }

    //temporary test obstacle
    obstacleMap[0][4] = true;
    obstacleMap[1][4] = true;
    obstacleMap[2][4] = true;
    obstacleMap[3][4] = true;
    obstacleMap[4][4] = true;
    obstacleMap[5][4] = true;
    obstacleMap[6][4] = true;
    obstacleMap[7][4] = true;
    obstacleMap[8][4] = true;

    obstacleMap[0][20] = true;
    obstacleMap[1][20] = true;
    obstacleMap[2][20] = true;
    obstacleMap[3][20] = true;
    obstacleMap[4][20] = true;
    obstacleMap[5][20] = true;
    obstacleMap[6][20] = true;
    obstacleMap[7][20] = true;
    obstacleMap[8][20] = true;


    obstacleMap[15][14] = true;
    obstacleMap[16][14] = true;
    obstacleMap[17][14] = true;
    obstacleMap[18][14] = true;
    obstacleMap[19][14] = true;
}

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

    // Map movement.
    for (int i = 0; i < groundLevel; i++) {
        for (int j = 0; j < viewWidth - 1; j++) {
            obstacleMap[i][j] = obstacleMap[i][j + 1];
        }
    }
    // Add missing column;
    for (int i = 0; i < groundLevel; i++) {
        obstacleMap[i][viewWidth - 1] = false;
    }

    // Update game elements like bird position, collision detection, etc.
    agent.update(birdPosition);
    // Ground and ceiling collision.
    if (birdPosition >= groundLevel || birdPosition <= 0) {
        running = false; // Game over if bird hits the ground or the ceiling
    }

    //Pillar collision.
    if (obstacleMap[birdPosition][0] == true) {
        running = false;
    }
}

void Simulator::render() {
    system("cls"); // Clear the console

    // Ceiling;
    for (int j = 0; j < viewWidth; j++) std::cout << "=";
    std::cout << "\n";
    for (int i = 0; i < groundLevel; i++) {
        std::cout << "  ";
        for (int j = 0; j < viewWidth; j++) {
            if (i == birdPosition && j == 0) std::cout << "O";
            else if (obstacleMap[i][j] == false) std::cout << " ";
            else if (obstacleMap[i][j] == true) std::cout << "#";
        }
        std::cout << "\n";
        //std::cout << i << ": ";
        /*if (i == birdPosition) {
            std::cout << "O\n"; // Bird
        }
        else {
            std::cout << "\n";
        }*/
    }
    // Ground;
    for (int j = 0; j < viewWidth; j++) std::cout << "=";
    std::cout << "\n";
}
