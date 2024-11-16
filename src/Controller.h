#pragma once
#include "Agent.h"
#include "Simulator.h"


class Controller
{
public:
    virtual bool action(Agent& agent, Simulator& simulator)
    {
        return false;
    }
};


class MyController : public Controller
{
public:
    bool action(Agent& agent, Simulator& simulator)
    {
        // get current position
        int yPos = agent.position * simulator.groundLevel;
        // is obstacle ahead
        bool obstacleAhead = false;
        simulator.obstacleMap;
        for (int i = 0; i < simulator.viewWidth; i++)
            if (simulator.obstacleMap[yPos][i] == true) {
                obstacleAhead = true;
                break;
            }
        // jump if obstacle ahead or too low, but not if too high
        if ((yPos > simulator.groundLevel * 0.35) && (obstacleAhead || yPos > simulator.groundLevel * 0.85)) {
            agent.isJumping = true;
            return true;
        }
        return false;
    }
};
