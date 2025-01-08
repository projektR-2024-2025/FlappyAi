#include "Controller.h"
#include "Agent.h"
#include "Simulator.h"
#include "FunctionBinaryTree.h"

class GPcontroller : public Controller{

public:
    FunctionBinaryTree f ;
    bool action_simple(Agent& agent, Simulator& simulator){
        int x = 0 ;
        // get current position
        int yPos = agent.position * simulator.groundLevel;
        // is obstacle ahead
        bool obstacleAhead = false;
        simulator.obstacleMap;
        for (int i = 0; i < simulator.viewWidth; i++){
            if (simulator.obstacleMap[yPos][i] == true) {
                obstacleAhead = true;
                break;
            }
        }
        vector<float> ulaz ;
        ulaz.push_back(yPos) ;
        ulaz.push_back(obstacleAhead) ;
        x = f.izracunaj(ulaz) ;
        if(x != 0){
            agent.isJumping = true ;
            return true ;
        }
        agent.isJumping = false ;
        return false ;
    }
    bool action(Agent& agent, Simulator& simulator){
        float x = 0 ;
        float obstacle_distance ;
        int hole_start=0; //y koordinata na kojoj pocinje rupa (zavrsava donja prepreka)
        int hole_end=0 ; //y koordiata na kojoj zavrsava rupa (pocinje gornja prepreka)
        // get current position
        float yPos = agent.position ;
        // is obstacle ahead
        bool obstacleAhead = false;
        simulator.obstacleMap;
        for (int i = 0; i < simulator.viewWidth; i++){
            for(int j = 0 ; j < simulator.groundLevel; j++){
                if (simulator.obstacleMap[j][i] == true) {
                    obstacle_distance = i ;
                    if(j != 0){ //nema donje prepreke
                        hole_start = 0 ;
                        hole_end = j ;
                    }
                    else{
                        while(simulator.obstacleMap[hole_start][i] == true && hole_start < simulator.groundLevel){
                            hole_start++ ;
                        }
                        hole_end = hole_start ;
                        while(simulator.obstacleMap[hole_end][i] == false && hole_start < simulator.groundLevel) {
                            hole_end++ ;
                        }
                    }
                }
            }    
        }
        vector<float> ulaz ;
        ulaz.push_back(yPos) ;
        ulaz.push_back(obstacle_distance / simulator.viewWidth) ;
        ulaz.push_back(static_cast<float>(hole_start) / simulator.groundLevel) ;
        ulaz.push_back(static_cast<float>(hole_end) / simulator.groundLevel) ;
        x = f.izracunaj(ulaz) ;
        if(abs(x) < 0.5){
            agent.isJumping = true ;
            return true ;
        }
        agent.isJumping = false ;
        return false ;
    }
};
