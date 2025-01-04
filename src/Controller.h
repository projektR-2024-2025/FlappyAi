#pragma once
#include "Agent.h"
#include "Simulator.h"
#include "CGP.h"
#include <vector>

class Controller
{
public:
    virtual bool action(Agent& agent, Simulator& simulator)
    {
        return false;
    }
};


class CGPController : public Controller
{
public:
    bool action(Agent& agent, Simulator& simulator, Individual& individual)
    {
        // input vector za cgp mrezu
        std::vector<double> input;

        // get current position
        double yPos = agent.position * simulator.groundLevel;
        // is obstacle ahead
        double obstacleAhead = 0;
        simulator.obstacleMap;
        for (int i = 0; i < VIEW_DISTANCE; i++)
            if (simulator.obstacleMap[(int) yPos][i] == true) {
                obstacleAhead = 1;
                break;
            }
        // stavi informacije u vektor
        input.push_back(obstacleAhead);
        input.push_back(agent.position);
        input.push_back(agent.velocity);
        input.push_back(simulator.groundLevel);
        
        // odredi vrijednost izlazne vrijednosti cgp mreze
        individual.evaluateValue(input);
        
        if (individual.outputGene[0].value > OUT_VALUE) {
            agent.isJumping = true;
            return true;
        }
        return false;
    }
};
