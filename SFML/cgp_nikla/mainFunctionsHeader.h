//
// Created by <3 on 13.01.2025..
//

#ifndef MAINFUNCTIONSHEADER_H
#define MAINFUNCTIONSHEADER_H
//
// Created by <3 on 13.01.2025..
//
#include <iostream>
#include <thread>
#include "Controller.h"
#include "Population.h"
#include "Simulator.h"
#include <fstream>
#include <cstdlib>


Entity runCgp(ActionType selectedAction) { // selectedAction => TRAIN BEST

    srand(time(NULL));
    //cout << "runs starting\n";
    //int x = 0;
    // string map1 = "/Users/nikson/Documents/FER/ProjektC/FlappyAICpp/Map1.txt";//jer je out folder pokrenut
    // string map2 = "/Users/nikson/Documents/FER/ProjektC/FlappyAICpp/Map2.txt";//jer je out folder pokrenut
    // string map3 = "/Users/nikson/Documents/FER/ProjektC/FlappyAICpp/Map3.txt";
    // string map4 = "/Users/nikson/Documents/FER/ProjektC/FlappyAICpp/Map4.txt";
    // string map5 = "/Users/nikson/Documents/FER/ProjektC/FlappyAICpp/Map5.txt";

    // vector<string> mapList = {map1, map2, map3, map4, map5};


    if(selectedAction == BEST) {
        Entity en = Entity();
        return en;
    }

    Population population1 = Population();



     for (int i = 0; i < Constants::NUMBER_OF_GENERATIONS; i++) {
         int newPopulationFullness;

         cout << i << "\n";
         if(i == 0){
             //punjenje populacije
             //filling and evaluating initial population ==============================
             for (int j = 0; j < Constants::POPULATION_SIZE; j++) {
                 Entity entity;
                 entity.createGenome();
                 //cout << "NUMBER_OF_GENERATIONS loop i is 0, POPULATION_SIZE loop j:" << j << "\n";
                 //entity.fitnessEval(lenthTraversed);//lenthTraversed nez odkud ovo dobit
                 population1.entityList[j] = entity;
             }
             //population1.entityFitnessEval();
             //population1.entityFitnessSort();
         }


         for (int j = 0; j < Constants::POPULATION_SIZE; j++) {
             //cout << "NUMBER_OF_GENERATIONS loop" << i <<", POPULATION_SIZE loop j:" << j << "\n";
             float fit = 0;
             for(auto& map : Parameters::maps) {
                 Bird agent = Bird();
                 Simulator simulator = Simulator(map);// treba konstruktor s mapom
                 simulator.initialize(agent);
                 CGPController2 controller = CGPController2(population1.entityList[j]);

                 while (simulator.isRunning() && population1.entityList.size() == Constants::POPULATION_SIZE) {

                     controller.action(agent, simulator);
                     simulator.update(agent);
                     //lengthTraversed++;

                     if (agent.distance >= Constants::MAX_MAP_SIZE) {
                         break;
                     }
                 }

                 fit = fit + agent.distance;
             }

             population1.entityList[j].fitness = fit/Parameters::maps.size();
         }

         Population population2 = Population();
         //population1.entityFitnessEval();
         population1.entityFitnessSort();
         //cout << "Best fitness so far is " << population1.getBestEntity().fitness << "\n";
         //izrada i punjenje nove populacije
         newPopulationFullness = 0;
         //x = 0;
         while (newPopulationFullness < Constants::POPULATION_SIZE && population1.entityList.size() == Constants::POPULATION_SIZE){
             //cout << "crossing over and mutation loop x:" << x << "\n";
             //x++;
             //cout << "a\n";
             population1.chooseParents();
             //cout << "b\n";
             newPopulationFullness = population1.crossingOverAndMutation(population2, newPopulationFullness);
             //cout << "c\n";
             newPopulationFullness++;
             //cout << "d\n";
         }
         population1.entityList.clear();
         //cout << "population 1 cleared\n";
         copy(population2.entityList.begin(), population2.entityList.end(), back_inserter(population1.entityList));
        // cout << "new population 1\n";

         cout << "fitness: "<<population1.getBestEntity().fitness << "\n";
     }
    //cout << "done\n";

    Entity bestEntity = population1.getBestEntity();


    if (selectedAction == TRAIN) {
        fstream myfile;
        myfile.open ("/Users/nikson/Documents/GitHub/FlappyAi/SFML/BestEntityFile");

        if (myfile.is_open())
        {
            myfile << bestEntity.toString();
            myfile.close();

        }else {
            cout << "Unable to open file(mainFunctionHeader)";
        }

    }



    cout << "cgp finished running\n";
    //exit(0);

    return bestEntity;
}




#endif //MAINFUNCTIONSHEADER_H
