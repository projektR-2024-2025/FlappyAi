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
using namespace std;

Population::Population() {
    entityList.reserve(Constants::POPULATION_SIZE);
    entityList.resize(Constants::POPULATION_SIZE);
}


void Population::entityFitnessSort() {
    sort(entityList.begin(), entityList.end(),
              [](Entity& e1, Entity& e2)
              {
                  return e1.getFitness() < e2.getFitness();
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


void Population::entityFitnessEval() {
    for (int i = 0; i < this->entityList.size(); i++){
        this->entityList[i].fitness = 1 - (this->entityList[i].lengthTraversed/Constants::MAX_MAP_SIZE);
    }
}


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
        int mutatingIndex1 = randomInt(0,Constants::ENTITY_SIZE);
        //cout << "mutating index1:" << mutatingIndex1 << "\n";

        int mutatingIndex2 = randomInt(0,Constants::ENTITY_SIZE);
        //cout << "mutating index2:" << mutatingIndex2 << "\n";

        if(mutatingIndex1 == 0 || mutatingIndex1 == 1){//prva dva gena, bira x ili y

            int a = randomInt(0,2);
            int b = randomInt(0,2);
            //cout << "a: " << a << " b: " << b << "\n";

            child1.genome[mutatingIndex1] = Gene(a,b);

        }else if(mutatingIndex1 == 2 || mutatingIndex1 == 3){//druga dva gena, bira x,y ili iz prvog stupca kucica

            int a = randomInt(0,4);
            int b = randomInt(0,4);
            //cout << "a: " << a << " b: " << b << "\n";

            child1.genome[mutatingIndex1] = Gene(a,b);

        }else if(mutatingIndex1 == 4 || mutatingIndex1 == 5){//treca dva gena, bira iz prva dva stupca kucica

            int a = randomInt(2,6);
            int b = randomInt(2,6);
            //cout << "a: " << a << " b: " << b << "\n";

            child1.genome[mutatingIndex1] = Gene(a,b);

        }else if (mutatingIndex1 == 6){//output, bira iz druga dva stupca kucica

            int outPut = randomInt(4,7);
            //cout << "output: " << outPut << "\n";

            child1.genome[mutatingIndex1] = Gene(outPut,outPut);
        }


        if(mutatingIndex2 == 0 || mutatingIndex2 == 1){//prva dva gena, bira x ili y

            int a = randomInt(0,2);
            int b = randomInt(0,2);
            //cout << "a: " << a << " b: " << b << "\n";

            child2.genome[mutatingIndex2] = Gene(a,b);

        }else if(mutatingIndex2 == 2 || mutatingIndex2 == 3){//druga dva gena, bira x,y ili iz prvog stupca kucica

            int a = randomInt(0,4);
            int b = randomInt(0,4);
            //cout << "a: " << a << " b: " << b << "\n";

            child2.genome[mutatingIndex2] = Gene(a,b);

        }else if(mutatingIndex2 == 4 || mutatingIndex2 == 5){//treca dva gena, bira iz prva dva stupca kucica

            int a = randomInt(2,6);
            int b = randomInt(2,6);
            //cout << "a: " << a << " b: " << b << "\n";

            child2.genome[mutatingIndex2] = Gene(a,b);

        }else if (mutatingIndex2 == 6){//output, bira iz druga dva stupca kucica
            int outPut = randomInt(4,7);
            //cout << "output: " << outPut << "\n";

            child2.genome[mutatingIndex2] = Gene(outPut,outPut);
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
