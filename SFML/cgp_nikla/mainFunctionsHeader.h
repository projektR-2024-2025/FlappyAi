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
#include "../Controller.h"
#include "Population.h"
#include "../Simulator.h"
#include <fstream>
#include <cstdlib>
#include "../LiberationSansFont.h"

Entity runCgp(ActionType selectedAction, sf::RenderWindow& window) { // selectedAction => TRAIN BEST

    srand(time(NULL));
    sf::Font font;
    font.loadFromMemory(&LiberationSans_Regular_ttf, LiberationSans_Regular_ttf_len);


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
         Entity bestEnt = population1.getBestEntity();

         trainingMenu(window, font, i, Constants::NUMBER_OF_GENERATIONS,population1.getBestEntity().fitness, "CGP2", new CGPController2(bestEnt));
         //trainingMenu(sf::RenderWindow& window, sf::Font& font, int generacija, int generations, float fitness, std::string model, Controller* controller);
     }
    //cout << "done\n";

    Entity bestEntity = population1.getBestEntity();


    if (selectedAction == TRAIN) {
        fstream myfile;
        myfile.open ("/Users/nikson/Documents/GitHub/FlappyAi/BestEntityFile.txt");

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







//=========================================================================

// CGP1Controller* CGP1::CGPMain(sf::RenderWindow& window) {
//     CGP1 cgp(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS, window);
//     CGP1Individual ind;
//
//     if (Parameters::action == BEST) {
//         try {
//             std::ifstream inFile(bestFile);
//             if (inFile.is_open()) {
//                 ind = CGP1Individual::deserialize(inFile);
//                 inFile.close();
//                 std::cout << "CGP read from text file." << std::endl;
//                 ind.printFuction();
//             }
//         }
//         catch (const exception& e) {
//             cerr << "Error loading CGP: " << e.what() << endl;
//         }
//     }
//     else if (Parameters::action == TRAIN) {
//         ind = cgp.runCGP();
//         ind.printFuction();
//     }
//
//     return new CGP1Controller(ind);
// }



// CGPController2* CGP2Main(sf::RenderWindow& window) {
//
//     Entity entity = runCgp(Parameters::action);
//
//     if(Parameters::action == TRAIN) {
//         entity = runCgp(Parameters::action);
//
//     }else if(Parameters::action == BEST) {
//
//         string bestEntityFilePath = "/Users/nikson/Documents/GitHub/FlappyAi/SFML/BestEntityFile";
//         entity = entity.stringToEntity(bestEntityFilePath);
//
//         cout << entity.toString()<<"\n";
//
//     }else {
//         cout << "Answer unvalid";
//     }
//
//
//     return new CGPController2(entity);
// }


//=========================================================================


#endif //MAINFUNCTIONSHEADER_H
