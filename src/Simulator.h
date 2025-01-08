#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include "Agent.h"
#include <string> 


class Simulator {
public:
    Simulator();
    void initializeMap(std::string file);
    void loadNextColumn(int pos = viewWidth - 1);

    bool isRunning() const;
    bool isScoreScreenActive() const;
    bool handleEvents(Agent& agent);
    void handleScoreEvents();
    void update(Agent& agent);
    void render();
    bool finishedMap() ;
    int getMapReadIndex() ;
    std::string file = "map3_5.txt" ;

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
    int birdPosition; //y
    int x_pos ;
    // Add other game elements like pipes here
};

#endif // SIMULATOR_H
