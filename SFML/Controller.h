#pragma once
#include "Agent.h"
#include "Simulator.h"
#include "parameters.h"
#include "./NeuralNetwork.h"
#include "CGP.h"
#include "Entity.h"
#include "CGPIndividual.h"

using namespace std;

class Controller
{
public:
    virtual bool action(Bird& agent, Simulator& simulator)
    {
        return false;
    }
};


class MyController : public Controller
{
public:
    bool action(Bird& bird, Simulator& simulator)
    {
        // heuristika: skoci ako je cijev ispred
        for (const auto& pipe : simulator.pipes) {
            if (pipe.x > 30 && pipe.x < 250) { // je li cijev blizu
                //if (bird.position + 40 > pipe.bottomY - PIPE_GAP / 2) { 
                if (bird.position + BIRD_SIZE > pipe.bottomY) { // je li ispred
                    bird.velocity = JUMP_SPEED;
                }
                break;
            }
        }
        return true;
    }
};

//class MyController : public Controller
//{
//public:
//    // dodaj model
//    bool action(Bird& agent, Simulator& simulator)
//    {
//        // get current position
//        int yPos = agent.position * simulator.groundLevel;
//        // is obstacle ahead
//        bool obstacleAhead = false;
//        simulator.obstacleMap;
//        for (int i = 0; i < simulator.viewWidth; i++)
//            if (simulator.obstacleMap[yPos][i] == true) {
//                obstacleAhead = true;
//                break;
//            }
//        // jump if obstacle ahead or too low, but not if too high
//        if ((yPos > simulator.groundLevel * 0.35) && (obstacleAhead || yPos > simulator.groundLevel * 0.85)) {
//            agent.isJumping = true;
//            return true;
//        }
//        return false;
//    }
//};

class NeuralController : public Controller
{
private:
    NeuralNetwork nn;

public:
    NeuralController(const NeuralNetwork& nn) : nn(nn) {}

    bool action(Bird& bird, Simulator& simulator)
    {
        // input vector za nn
        std::vector<double> input;
        // is obstacle ahead
        double obstacleAhead = 0;
        for (const auto& pipe : simulator.pipes)
            if (pipe.x > 30 && pipe.x < 250) {
                if (bird.position + BIRD_SIZE > pipe.bottomY) {
                    obstacleAhead = 1;
                }
                break;
            }

        // priprema input data
        input.push_back(obstacleAhead);
        input.push_back(bird.position);
        input.push_back(bird.velocity);
        input.push_back(simulator.groundLevel);

        // dobi outpute iz nn
        std::vector<double> output = nn.predict(input);

        // koristi nn outpute za odredjivanje actiona
        //std::cout << output[0] << " " << output[1] << std::endl;
        if ((output[0] + output[1]) / 2.f > 0.60) {
            bird.velocity = JUMP_SPEED;
        }
        return true;
        /*if (output[0] < output[1]) {
            agent.isJumping = true;
        }
        else {
            agent.isJumping = false;
        }
        return agent.isJumping;*/
    }
};

class CGPController2 : public Controller {
private:
    Entity entity;

public:

    CGPController2(const Entity& entity) : entity(entity) {}

    CGPController2(const Entity& learnedEntity, bool simOnly) {
        if(simOnly == false) {
            this->entity = learnedEntity;

        }else if(simOnly == true) {
            string lineFromFile;
            stringstream lineStream;
            vector<string> splicedString;
            string segment;
            fstream myFile;

            myFile.open ("/Users/nikson/Documents/GitHub/FlappyAi/SFML/BestEntityFile");

            if (myFile.is_open())
            {
                getline(myFile, lineFromFile);
                myFile.close();
            }

            lineStream = (stringstream)lineFromFile;

            while(std::getline(lineStream, segment, ' '))
            {
                splicedString.push_back(segment);
            }
            this->entity = this->entity.stringToEntity(splicedString);

            cout << this->entity.toString()<<"\n";


        }else {
            cout << "Answer unvalid";
        }



    }

    bool action(Bird& agent, Simulator& simulator)
    {
        vector<double> input(Constants::AMOUNT_OF_CGP_INPUTS);

        bool obstacleAhead = false;
        for (const auto& pipe : simulator.pipes)
            if (pipe.x > 30 && pipe.x < 250) {
                if (agent.position + BIRD_SIZE > pipe.bottomY) {
                    obstacleAhead = 1;
                }
                break;
            }

        input[0] = obstacleAhead ? 1.0 : 0.0;
        input[1] = agent.position;
        input[2] = agent.velocity;
        input[3] = simulator.groundLevel;

        double output = entity.entityFunction(input);

        if(output > Constants::DO_I_JUMP) {
            agent.isJumping = true;
            return true;
        }
        return false;
    }
};


class CGPController : public Controller
{
private:
    CGPIndividual individual;
public:
    CGPController(const CGPIndividual& individual) : individual(individual) {}


    bool action(Bird& bird, Simulator& simulator)
    {
        // input vector za cgp mrezu
        std::vector<double> input;

        // get current position
        // is obstacle ahead
        double obstacleAhead = 0;
        for (const auto& pipe : simulator.pipes)
            if (pipe.x > 30 && pipe.x < 250) {
                if (bird.position + BIRD_SIZE > pipe.bottomY) {
                    obstacleAhead = 1;
                }
                break;
            }
        // stavi informacije u vektor
        input.push_back(obstacleAhead);
        input.push_back(bird.position);
        input.push_back(bird.velocity);

        // odredi vrijednost izlazne vrijednosti cgp mreze
        individual.evaluateValue(input);

        if (!isnan(individual.outputGene[0].value) && individual.outputGene[0].value > OUT_VALUE)
            bird.velocity = JUMP_SPEED;
        return true;
    }
};
