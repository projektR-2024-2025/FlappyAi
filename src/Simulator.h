#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Agent.h"

class Simulator {
public:
    Simulator();
    bool isRunning() const;
    bool isScoreScreenActive() const;
    void handleEvents(Agent& agent);
    void handleScoreEvents();
    void update(Agent& agent);
    void render();

private:
    bool running;
    bool scoreScreen;
    int birdPosition;
    int groundLevel;
    // Add other game elements like pipes here
};

#endif // SIMULATOR_H
