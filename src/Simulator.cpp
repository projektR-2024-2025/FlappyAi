#include "Simulator.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <conio.h> // For _kbhit() and _getch()

Simulator::Simulator(int mapNum) : running(true), scoreScreen(true) {
    for (int i = 0; i < groundLevel; i++) {
        for (int j = 0; j < viewWidth; j++) {
            obstacleMap[i][j] = false;
        }
    }
    std::string mapFileName = "";
    if (mapNum == 1) mapFileName = "map1.txt";
    else if (mapNum == 2) mapFileName = "map2.txt";
    else mapFileName = "map3.txt";
    initializeMap(mapFileName);
}

bool Simulator::isRunning() const {
    return running;
}

bool Simulator::isScoreScreenActive() const {
    return scoreScreen;
}

bool Simulator::handleEvents(Agent& agent) {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 'q') {
            running = false;
        }
        else {
            agent.handleInput(ch);
        }
        return true;
    }
    return false;
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
    if (birdPosition >= groundLevel || birdPosition < 0) {
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
void Simulator::initializeMap(std::string map) {
    // Load map from file.
    std::ifstream file(map);

    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ' ')) {
            tokens.push_back(token);
        }
        mapEmpty.push_back(std::stoi(tokens[0]));
        mapCeiling.push_back(std::stoi(tokens[1]));
        mapGround.push_back(std::stoi(tokens[2]));
    }

    // Close the file
    file.close();

    mapLen = mapLenght();

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

int Simulator::mapLenght() {
    int mapLen = 0;
    for (int i = 0; i < mapEmpty.size(); i++){
        mapLen += mapEmpty[i];
        mapLen += 2;
    }
    return mapLen;
}
