//
// Created by <3 on 08.01.2025..
//

//NE SVIDA MI SE OVAJ PUSHBACK
//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//

#include "Population.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <algorithm>
using namespace std;

Population::Population() {
    entityList.reserve(Constants::POPULATION_SIZE);
    entityList.resize(Constants::POPULATION_SIZE);
}

Population::~Population() {}



void Population::entityFitnessSort() {
    sort(entityList.begin(), entityList.end(),
              [](Entity& e1, Entity& e2)
              {
                  return e1.getFitness() > e2.getFitness();
              });
}

Entity &Population::getBestEntity() {
    return entityList[0];
}


void Population::chooseParents() {
    //cout << "choose parents function\n";
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    int halfway = Constants::POPULATION_SIZE / 2;

    //cout << "index for choosing parents:" << r << "   halfway point:" << halfway << "\n";

    if(r < Constants::BETTER_PARENT_ADVANTAGE) {
        int r1 = randomInt(0, halfway - 1);
        int r2 = randomInt(0,halfway - 1);

        //cout << "r1: " << r1 << "\n";
        //cout << "r2: " << r2 << "\n";

        parent1 = entityList[r1];
        parent2 = entityList[r2];

    }else if(r >= Constants::BETTER_PARENT_ADVANTAGE) {
        int r1 = randomInt(halfway, Constants::POPULATION_SIZE - 1);
        int r2 = randomInt(halfway,Constants::POPULATION_SIZE -1 );

        //cout << "r1: " << r1 << "\n";
        //cout << "r2: " << r2 << "\n";

        parent1 = entityList[r1];
        parent2 = entityList[r2];
    }
}


// void Population::entityFitnessEval() {
//     for (int i = 0; i < this->entityList.size(); i++){
//         this->entityList[i].fitness = this->entityList[i].lengthTraversed/;
//     }
// }


int Population::crossingOverAndMutation(Population &NewPop, int index) {
    //vraca zadnji idex na koji je nesto stavljeno u novu populaciju ??

    if(index == 0){
        NewPop.entityList[0] = this->getBestEntity();//dodamo najbolju jedinku u novu populaciju
        // ovo tu bi moglo van ic myb da ovaj idex krece od 1??
        return index;
    }

    int breakPoint = Constants::ENTITY_SIZE / 2;

    Entity child1 = Entity();
    Entity child2= Entity();

    for(int i = 0; i < breakPoint; i++) {
        child1.genome[i] = this->parent1.genome[i];
        child2.genome[i] = this->parent2.genome[i];
    }

    for(int i = breakPoint; i < Constants::ENTITY_SIZE; i++) {
        child1.genome[i] = this->parent2.genome[i];
        child2.genome[i] = this->parent1.genome[i];
    }

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    //cout << "mutation probability r:" << r << "\n";

    if(r < Constants::MUTATION_PROBABILITY) {
        int a;
        int b;

        int mutatingGeneAmount = Constants::ENTITY_SIZE * Constants::PERCENTAGE_OF_GENES;

        for(int i = 0; i < mutatingGeneAmount; i++) {
            int mutatingIndex1 = randomInt(0,Constants::ENTITY_SIZE - 1);
            int mutatingIndex2 = randomInt(0,Constants::ENTITY_SIZE - 1);


            if(mutatingIndex1 == 0 || mutatingIndex1 == 1){//prva dva gena, bira x ili y

                a = randomInt(0, Constants::AMOUNT_OF_CGP_INPUTS - 1);
                b = randomInt(0,Constants::AMOUNT_OF_CGP_INPUTS - 1);
                //cout << "a: " << a << " b: " << b << "\n";

                child1.genome[mutatingIndex1] = Gene(a,b);

            }else if(mutatingIndex1 == 2 || mutatingIndex1 == 3){//druga dva gena, bira x,y ili iz prvog stupca kucica

                a = randomInt(0, Constants::AMOUNT_OF_CGP_INPUTS + 1);
                b = randomInt(0,Constants::AMOUNT_OF_CGP_INPUTS + 1);

                child1.genome[mutatingIndex1] = Gene(a,b);

            }else if(mutatingIndex1 >= 4 && mutatingIndex1 < Constants::ENTITY_SIZE - 1) {
                if(mutatingIndex1 % 2 == 0) {
                    a = randomInt(mutatingIndex1, mutatingIndex1 + 3);
                    b = randomInt(mutatingIndex1 ,mutatingIndex1 + 3);

                }else if (mutatingIndex1 % 2 == 1) {
                    a = randomInt(mutatingIndex1 - 1, mutatingIndex1 + 2);
                    b = randomInt(mutatingIndex1 - 1 ,mutatingIndex1 + 2);
                }

                child1.genome[mutatingIndex1] = Gene(a,b);

            }else if (mutatingIndex1 == Constants::ENTITY_SIZE - 1){//output, bira iz druga dva stupca kucica

                int outPut = randomInt(Constants::ENTITY_SIZE - 3, Constants::ENTITY_SIZE - 2);
                child1.genome[mutatingIndex1] = Gene(outPut,outPut);
            }


            if(mutatingIndex2 == 0 || mutatingIndex2 == 1){//prva dva gena, bira x ili y

                a = randomInt(0, Constants::AMOUNT_OF_CGP_INPUTS - 1);
                b = randomInt(0,Constants::AMOUNT_OF_CGP_INPUTS - 1);
                //cout << "a: " << a << " b: " << b << "\n";

                child2.genome[mutatingIndex2] = Gene(a,b);

            }else if(mutatingIndex2 == 2 || mutatingIndex2 == 3){//druga dva gena, bira x,y ili iz prvog stupca kucica

                a = randomInt(0, Constants::AMOUNT_OF_CGP_INPUTS + 1);
                b = randomInt(0,Constants::AMOUNT_OF_CGP_INPUTS + 1);

                child2.genome[mutatingIndex2] = Gene(a,b);

            }else if(mutatingIndex2 >= 4 && mutatingIndex2 < Constants::ENTITY_SIZE - 1) {
                if(mutatingIndex2 % 2 == 0) {
                    a = randomInt(mutatingIndex2, mutatingIndex2 + 3);
                    b = randomInt(mutatingIndex2 ,mutatingIndex2 + 3);

                }else if (mutatingIndex2 % 2 == 1) {
                    a = randomInt(mutatingIndex2 - 1, mutatingIndex2 + 2);
                    b = randomInt(mutatingIndex2 - 1 ,mutatingIndex2 + 2);
                }

                child1.genome[mutatingIndex2] = Gene(a,b);

            }else if (mutatingIndex2 == Constants::ENTITY_SIZE - 1){//output, bira iz druga dva stupca kucica

                int outPut = randomInt(Constants::ENTITY_SIZE - 3, Constants::ENTITY_SIZE - 2);
                child1.genome[mutatingIndex2] = Gene(outPut,outPut);
            }
        }

    }else{
        //cout << "not mutating\n";
    }

    NewPop.entityList[index] = child1;
    index++;


    if(index != Constants::POPULATION_SIZE) {
        NewPop.entityList[index] = child2;
    }
    //cout << "exiting CO & Mutation\n";
    return index;
}
