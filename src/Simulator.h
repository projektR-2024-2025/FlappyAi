#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include "Agent.h"

class Simulator {
public:
    Simulator();
    void initializeMap();
    void loadNextColumn(int pos = viewWidth - 1);

    bool isRunning() const;
    bool isScoreScreenActive() const;
    void handleEvents(Agent& agent);
    void handleScoreEvents();
    void update(Agent& agent);
    void render();

private:
    std::vector<int> mapEmpty;
    std::vector<int> mapCeiling;
    std::vector<int> mapGround;
    int mapReadIndex = 0;
    bool pillarFlag = 0;

    bool running;
    bool scoreScreen;
    int birdPosition;
    static constexpr int groundLevel = 20;
    static constexpr int viewWidth = 50;
    bool obstacleMap[groundLevel][viewWidth];
    // Add other game elements like pipes here
};

#endif // SIMULATOR_H
