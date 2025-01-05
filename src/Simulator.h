#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <string>
#include "Agent.h"
#include "CGP.h"

class Simulator {
public:
    CGP cgp;

    Simulator();
    Simulator(std::string map);
    void initializeMap(std::string map);
    void loadNextColumn(int pos = viewWidth - 1);

    bool isRunning() const;
    bool isScoreScreenActive() const;
    bool handleEvents(Agent& agent);
    void handleScoreEvents();
    void update(Agent& agent);
    void render();

    static constexpr int groundLevel = 20;
    static constexpr int viewWidth = 35;
    bool obstacleMap[groundLevel][viewWidth];

private:
    std::vector<int> mapEmpty;
    std::vector<int> mapCeiling;
    std::vector<int> mapGround;
    int mapReadIndex = 0;
    bool pillarFlag = 0;

    bool running;
    bool scoreScreen;
    int birdPosition;
    // Add other game elements like pipes here
};

#endif // SIMULATOR_H
