#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Agent.h"

class Simulator {
public:
    Simulator();
    bool isRunning() const;
    void handleEvents(Agent& agent);
    void update(Agent& agent);
    void render();

private:
    bool running;
    int birdPosition;
    int groundLevel;
    // Add other game elements like pipes here
};

#endif // SIMULATOR_H