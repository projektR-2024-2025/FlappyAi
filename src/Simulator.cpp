#include "Simulator.h"
#include <iostream>
#include <conio.h> // For _kbhit() and _getch()

Simulator::Simulator() : running(true), scoreScreen(true) {
    for (int i = 0; i < groundLevel; i++) {
        for (int j = 0; j < viewWidth; j++) {
            obstacleMap[i][j] = false;
        }
    }

    initializeMap();
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
    /*for (int i = 0; i < groundLevel; i++) {
        obstacleMap[i][viewWidth - 1] = false;
    }*/
    loadNextColumn();

    // Update game elements like bird position, collision detection, etc.
    double y = agent.update(birdPosition);
    // calculate integer birdPosition
    birdPosition = (int) (y * groundLevel);
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


// Map initialization.
// za sad je smao placeholder
void Simulator::initializeMap() {
            // placeholder pillar 1
            mapEmpty.push_back(20);
            mapCeiling.push_back(3);
            mapGround.push_back(5);

            // placeholder pillar 2
            mapEmpty.push_back(30);
            mapCeiling.push_back(7);
            mapGround.push_back(1);

            // placeholder pillar 3
            mapEmpty.push_back(25);
            mapCeiling.push_back(8);
            mapGround.push_back(6);

    // Load map from file.

    // Fill the initial view.
    for (int j = 0; j < viewWidth; j++) {
        loadNextColumn(j);
    }
}

// Next Column loader
void Simulator::loadNextColumn(int pos) {
    if (mapReadIndex >= mapEmpty.size()) {
        for (int i = 0; i < groundLevel; i++) {
            obstacleMap[i][pos] = false;
        }
    }
    else if (pillarFlag) {
        pillarFlag = false;
        for (int i = 0; i < groundLevel; i++) {
            obstacleMap[i][pos] = false;
            if (i <= mapCeiling[mapReadIndex] - 1 || i >= groundLevel - mapGround[mapReadIndex]) obstacleMap[i][pos] = true;
        }
        mapReadIndex += 1;
    }
    else if (mapEmpty[mapReadIndex] == 0) {
        for (int i = 0; i < groundLevel; i++) {
            obstacleMap[i][pos] = false;
            if (i <= mapCeiling[mapReadIndex] - 1 || i >= groundLevel - mapGround[mapReadIndex]) obstacleMap[i][pos] = true;
        }
        pillarFlag = true;
    }
    else {
        for (int i = 0; i < groundLevel; i++) {
            obstacleMap[i][pos] = false;
        }
        mapEmpty[mapReadIndex] -= 1;
    }
}
