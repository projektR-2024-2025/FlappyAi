#pragma once
#include "Agent.h"
#include "Simulator.h"
#include "TreeControll.h"


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
        //simulator.obstacleMap;
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

class MyTestController : public Controller
{
public:
    bool action(Agent& agent, Simulator& simulator) {
        agent.isJumping = true;
    }
};


class GPContreoller : public Controller
{
public:
    Node * passedRoot;
    double distFlown;

    bool action(Agent& agent, Simulator& simulator)
    {
        // get current y position 18
        yPos = agent.position * simulator.groundLevel;

        // get current velocity 17
        velocity = agent.velocity * simulator.groundLevel;


        // get next pillar dist 14 NA DALJE KRIVO RADI SA OBSTACLE MAP
        nextPillarDist = -1;
        for (int j = 0; j < simulator.viewWidth; j++) {
            bool obstacleHere = false;
            for (int i = 0; i < simulator.groundLevel; i++){
                if (simulator.obstacleMap[i][j] == true) obstacleHere = true;
            }
            if (!obstacleHere) nextPillarDist += 1;
            else break;
        }

        // Check if above next bottom 15
        int lastBotY = simulator.groundLevel - 1;
        for (int i = simulator.groundLevel - 1; i >= 0; i--) {
            if (simulator.obstacleMap[i][nextPillarDist + 1] == true) lastBotY = i;
            else break;
        }
        aboveNxtBot = yPos < lastBotY;

        // Check if bellow next top 16
        int lastTopY = 0;
        for (int i = 0; i < simulator.groundLevel; i++) {
            if (simulator.obstacleMap[i][nextPillarDist + 1] == true) lastTopY = i;
            else break;
        }
        bellowNxtTop = yPos > lastTopY;

        double calcResult = calcNode(passedRoot);

        /*
        passedRoot->printNode();
        std::cout << "\nIn this iteration:\n";
        std::cout << "    nextPillarDist = " << nextPillarDist << "\n";
        std::cout << "    aboveNxtBot = " << aboveNxtBot << "\n";
        std::cout << "    bellowNxtTop = " << bellowNxtTop << "\n";
        std::cout << "    velocity = " << velocity << "\n";
        std::cout << "    yPos = " << yPos << "\n";
        std::cout << calcResult << "\n";
        */

        // Ovdje bi trebala ic evaluacija stabla i funkcija koja racuna odluku o skakanju.

        agent.isJumping = (bool) calcResult;
        return true;
    }

    double calcNode(Node* currNode) {
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
        else return 0;

    }
private:
    int yPos;
    int velocity;
    int nextPillarDist;
    bool aboveNxtBot;
    bool bellowNxtTop;
};
