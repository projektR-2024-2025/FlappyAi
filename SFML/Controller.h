#pragma once
#include "Agent.h"
#include "Simulator.h"
#include "Parameters.h"
#include "./nn/NeuralNetwork.h"
#include "cgp_andrija/CGP1Individual.h"
#include <cmath>
#include "./gp_tonka/FunctionBinaryTree.h"
#include "./gp_filip/TreeControll.h"

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
                //if (bird.position + 40 > pipe.bottomY - Parameters::PIPE_GAP / 2) {
                if (bird.position + Parameters::BIRD_SIZE > pipe.bottomY) { // je li ispred
                    bird.velocity = Parameters::JUMP_SPEED;
                }
                break;
            }
        }
        return true;
    }
};

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

        // get current position
        // is obstacle ahead
        double obstacleAhead = 0;
        for (const auto& pipe : simulator.pipes)
            if (pipe.x > 30 && pipe.x < 250) {
                if (bird.position + Parameters::BIRD_SIZE > pipe.bottomY) {
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
            bird.velocity = Parameters::JUMP_SPEED;
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

class CGP1Controller : public Controller
{
private:
    CGP1Individual individual;
public:
    CGP1Controller(const CGP1Individual& individual) : individual(individual) {}

    bool action(Bird& bird, Simulator& simulator)
    {
        // input vector za cgp mrezu
        std::vector<double> input;

        input.push_back(0);
        input.push_back(0);
        input.push_back(0);

        // get current position
        // is obstacle ahead
        double obstacleAhead = 0;
        for (const auto& pipe : simulator.pipes)
            if (pipe.x > 30 && pipe.x < 250) {
                input[0] = (pipe.x);
                input[1] = (pipe.bottomY);
                input[2] = (pipe.topY);
                if (bird.position + Parameters::BIRD_SIZE > pipe.bottomY || bird.position < pipe.topY) {
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

        if (!isnan(individual.outputGene[0].value) && individual.outputGene[0].value > 0)
            bird.velocity = Parameters::JUMP_SPEED;
        return true;
    }
};

class GPcontroller : public Controller{

public:
    GPcontroller(FunctionBinaryTree f2): f(f2) {};

    FunctionBinaryTree f ;

    bool action(Bird& bird, Simulator& simulator){

        float obstacle_distance = simulator.pipes[0].x;
        float hole_start=simulator.pipes[0].bottomY;
        float hole_end=simulator.pipes[0].topY;

        //trazimo najblizu prepreku
        for (const auto& pipe : simulator.pipes) {
            if (pipe.x > 0 && pipe.x < 250){
                obstacle_distance = pipe.x ;
                hole_start=pipe.bottomY ;
                hole_end=pipe.topY ;
                break ;
            }
        }

        float yPos = bird.position ;

        vector<float> ulaz ;
        ulaz.push_back(yPos) ;
        ulaz.push_back(obstacle_distance) ;
        ulaz.push_back(hole_start) ;
        ulaz.push_back(hole_end) ;

        float x = f.izracunaj(ulaz) ;
        if(abs(x) < 0.5){
            bird.velocity = Parameters::JUMP_SPEED;
            return true ;
        }

        return false ;
    }
};

class GP2Controller : public Controller
{
public:
    GP2Controller(Node2* passedRoot): passedRoot(passedRoot){};

    Node2 * passedRoot;
    //double distFlown;

    bool action(Bird& bird, Simulator& simulator)
    {
        // get current y position 18
        yPos = bird.position;

        // get current velocity 17
        velocity = bird.velocity;


        // get next pillar dist 14 NA DALJE KRIVO RADI SA OBSTACLE MAP
        nextPillarDist = -1;
        nextPillarIndex = -1;
        for (int i = 0; i < simulator.pipes.size();i++) {
            Pipe pipe = simulator.pipes[i];
            if (pipe.x  > Parameters::BIRD_OFFSET) {
                nextPillarDist = pipe.x - Parameters::BIRD_OFFSET;
                nextPillarIndex = i;
                break;
            }
        }


        // Check if above next bottom 15
        aboveNxtBot = true;
        if (nextPillarIndex != -1) {
            Pipe pipe = simulator.pipes[nextPillarIndex];
            aboveNxtBot = bird.position + Parameters::BIRD_SIZE < pipe.bottomY;
        }

        // Check if bellow next top 16
        bellowNxtTop = true;
        if (nextPillarIndex != -1) {
            Pipe pipe = simulator.pipes[nextPillarIndex];
            bellowNxtTop = bird.position > pipe.topY;
        }

        // 19
        obstacleAhead = false;
        for (const auto& pipe : simulator.pipes)
            if (pipe.x > 30 && pipe.x < 250) {
                if (bird.position + Parameters::BIRD_SIZE > pipe.bottomY || bird.position < pipe.topY) {
                    obstacleAhead = true;
                }
                break;
        }

        double calcResult = calcNode(passedRoot);

        if ((bool) calcResult) bird.velocity = Parameters::JUMP_SPEED;
        return true;
    }

    double calcNode(Node2* currNode) {
        double sol;
        if (currNode == nullptr) return 0;
        else if (currNode->value == 0) return calcNode(currNode->left) + calcNode(currNode->right);
        else if (currNode->value == 1) return calcNode(currNode->left) - calcNode(currNode->right);
        else if (currNode->value == 2) return calcNode(currNode->left) * calcNode(currNode->right);
        else if (currNode->value == 3) return calcNode(currNode->left) / calcNode(currNode->right);
        else if (currNode->value == 4) return calcNode(currNode->left) < calcNode(currNode->right);
        else if (currNode->value == 5) return calcNode(currNode->left) > calcNode(currNode->right);
        else if (currNode->value == 6) return calcNode(currNode->left) <= calcNode(currNode->right);
        else if (currNode->value == 7) return calcNode(currNode->left) >= calcNode(currNode->right);
        else if (currNode->value == 8) return calcNode(currNode->left) == calcNode(currNode->right);
        else if (currNode->value == 9) return calcNode(currNode->left) && calcNode(currNode->right);
        else if (currNode->value == 10) return calcNode(currNode->left) || calcNode(currNode->right);
        else if (currNode->value == 11) return !(calcNode(currNode->right));
        else if (currNode->value == 12) return 0;
        else if (currNode->value == 13) return 1;
        else if (currNode->value == 14) return nextPillarDist;
        else if (currNode->value == 15) return aboveNxtBot;
        else if (currNode->value == 16) return bellowNxtTop;
        else if (currNode->value == 17) return velocity;
        else if (currNode->value == 18) return yPos;
        else if (currNode->value == 19) return obstacleAhead;
        else return 0;

    }
private:
    int yPos;
    int velocity;

    float nextPillarDist;
    int nextPillarIndex;

    bool aboveNxtBot;
    bool bellowNxtTop;
    bool obstacleAhead;
};

