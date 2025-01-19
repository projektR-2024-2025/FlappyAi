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


Population runCgp() {

    srand(time(NULL));
    cout << "runs starting\n";
    //string map1 = "/Users/nikson/Documents/FER/ProjektC/FlappyAICpp/Map1.txt";//jer je out folder pokrenut
    //string map2 = "/Users/nikson/Documents/FER/ProjektC/FlappyAICpp/Map2.txt";//jer je out folder pokrenut


    Population population1 = Population();


     for (int i = 0; i < Constants::NUMBER_OF_GENERATIONS; i++) {
         int newPopulationFullness;

         if(i == 0){
             //punjenje populacije
             //filling and evaluating initial population ==============================
             for (int j = 0; j < Constants::POPULATION_SIZE; j++) {
                 Entity entity;
                 entity.createGenome();
                 //entity.fitnessEval(lenthTraversed);//lenthTraversed nez odkud ovo dobit
                 population1.entityList[j] = entity;
             }
             //population1.entityFitnessEval();
             //population1.entityFitnessSort();
         }


         for (int j = 0; j < Constants::POPULATION_SIZE; j++) {
             int lengthTraversed = 0;
             Simulator simulator = Simulator();
             Bird agent = Bird();
             CGPController2 controller = CGPController2(population1.entityList[j]);

             while (simulator.isRunning() && population1.entityList.size() == Constants::POPULATION_SIZE) {

                 controller.action(agent, simulator);
                 simulator.update(agent);
                 lengthTraversed++;

                 if (lengthTraversed >= Constants::MAX_MAP_SIZE) {
                     break;
                 }
                 population1.entityList[j].lengthTraversed = lengthTraversed;
             }
         }

         Population population2 = Population();
         population1.entityFitnessEval();
         population1.entityFitnessSort();
         cout << "Best fitness so far is " << population1.getBestEntity().fitness << "\n";
         //izrada i punjenje nove populacije
         newPopulationFullness = 0;

         while (newPopulationFullness < Constants::POPULATION_SIZE && population1.entityList.size() == Constants::POPULATION_SIZE){
             population1.chooseParents();
             newPopulationFullness = population1.crossingOverAndMutation(population2, newPopulationFullness);
             newPopulationFullness++;
         }

         //population1.entityList = population2.entityList;
         // population2.entityFitnessEval();
         // population2.entityFitnessSort();
         population1.entityList.clear();
         // cout << "population";

         // population1.entityList.resize(Constants::POPULATION_SIZE);
         // cout << "population";
         copy(population2.entityList.begin(), population2.entityList.end(), back_inserter(population1.entityList));
         // population1.entityFitnessEval();
         // population1.entityFitnessSort();
         // cout<< "vectors";

     }

    return population1;
}




#endif //MAINFUNCTIONSHEADER_H
