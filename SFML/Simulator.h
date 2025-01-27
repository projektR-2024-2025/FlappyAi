#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Agent.h"

struct Pipe {
    float x;        // udaljenost
    float topY;     // gornji rub
    float bottomY;  // donji rub
};


class Simulator {
public:
    Simulator();
    ~Simulator();
    Simulator(sf::RenderWindow* window);
    Simulator(std::string map);
    void initializeMap(std::string map);
    void loadNextColumn(int pos = viewWidth - 1);

    bool initialize(Bird& agent);
    void initializeMap(Bird& agent);
    bool simulateFrame(Bird& bird, float deltaTime);

    bool isRunning() const;
    bool isScoreScreenActive() const;
    bool handleEvents(Bird& agent);
    void handleScoreEvents();
    void update(Bird& agent);
    void render();

    static constexpr int groundLevel = 20;
    static constexpr int viewWidth = 35;
    bool obstacleMap[groundLevel][viewWidth];

    std::vector<Pipe> pipes;
    sf::RenderWindow* window;
    sf::Clock clock;
    sf::RectangleShape birdShape;
    sf::RectangleShape topPipe;
    sf::RectangleShape bottomPipe;
    sf::RectangleShape ground;

private:
    std::vector<int> mapEmpty;
    std::vector<int> mapCeiling;
    std::vector<int> mapGround;
    int mapReadIndex = 0;
    bool pillarFlag = 0;
    //std::string map = "Map1.txt";
    std::string map = "/Users/nikson/Documents/GitHub/FlappyAi/SFML/Map1.txt";

    bool running;
    bool scoreScreen;
    int birdPosition;
};

#endif // SIMULATOR_H
