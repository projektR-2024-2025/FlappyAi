#pragma once
#include "Agent.h"
#include "Simulator.h"
#include "../../Ptica/Ptica/NeuralNetwork.h"


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
    // dodaj model
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

class NeuralController : public Controller
{
private:
    NeuralNetwork nn;

public:
    NeuralController(const NeuralNetwork& nn) : nn(nn) {}

    bool action(Agent& agent, Simulator& simulator)
    {
        // input vector za nn
        std::vector<double> input;

        // get current position
        double yPos = agent.position * simulator.groundLevel;
        // is obstacle ahead
        double obstacleAhead = 0;
        simulator.obstacleMap;
        for (int i = 0; i < 12; i++) {
            if (simulator.obstacleMap[(int)yPos][i] == true) {
                obstacleAhead = 1;
                break;
            }
        }

        // priprema input data
        input.push_back(obstacleAhead);
        input.push_back(agent.position);
        input.push_back(agent.velocity);
        input.push_back(simulator.groundLevel);

        // dobi outpute iz nn
        std::vector<double> output = nn.predict(input);

        // koristi nn outpute za odredjivanje actiona
        //std::cout << output[0] << " " << output[1] << std::endl;
        if ((output[0] + output[1]) / 2.f > 0.33) {
            agent.isJumping = true;
        }
        else {
            agent.isJumping = false;
        }
        return agent.isJumping;
        /*if (output[0] < output[1]) {
            agent.isJumping = true;
        }
        else {
            agent.isJumping = false;
        }
        return agent.isJumping;*/
    }
};