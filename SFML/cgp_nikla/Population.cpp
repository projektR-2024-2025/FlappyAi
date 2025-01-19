//
// Created by <3 on 08.01.2025..
//

//NE SVIDA MI SE OVAJ PUSHBACK
//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//

#include "Population.h"
#include "Constants.h"
#include <random>
#include <cmath>


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
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    int halfway = Constants::POPULATION_SIZE / 2;

    if(r < Constants::BETTER_PARENT_ADVANTAGE) {
        int r1 = randomInt(0, halfway);
        int r2 = randomInt(0,halfway);

        parent1 = entityList[r1];
        parent2 = entityList[r2];

    }else if(r >= Constants::BETTER_PARENT_ADVANTAGE) {
        int r1 = randomInt(halfway, Constants::POPULATION_SIZE);
        int r2 = randomInt(halfway,Constants::POPULATION_SIZE);

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

    if(r < Constants::MUTATION_PROBABILITY) {
        int mutatingIndex1 = randomInt(0,Constants::ENTITY_SIZE);
        int mutatingIndex2 = randomInt(0,Constants::ENTITY_SIZE);

        if(mutatingIndex1 == 0 || mutatingIndex1 == 1){//prva dva gena, bira x ili y
            child1.genome[mutatingIndex1] = Gene(randomInt(0,2),randomInt(0,2));

        }else if(mutatingIndex1 == 2 || mutatingIndex1 == 3){//druga dva gena, bira x,y ili iz prvog stupca kucica
            child1.genome[mutatingIndex1] = Gene(randomInt(0,4),randomInt(0,4));

        }else if(mutatingIndex1 == 4 || mutatingIndex1 == 5){//treca dva gena, bira iz prva dva stupca kucica
            child1.genome[mutatingIndex1] = Gene(randomInt(2,6),randomInt(2,6));

        }else if (mutatingIndex1 == 6){//output, bira iz druga dva stupca kucica
            int outPut = randomInt(4,7);
            child1.genome[mutatingIndex1] = Gene(outPut,outPut);
        }

        if(mutatingIndex2 == 0 || mutatingIndex2 == 1){//prva dva gena, bira x ili y
            child2.genome[mutatingIndex2] = Gene(randomInt(0,2),randomInt(0,2));

        }else if(mutatingIndex2 == 2 || mutatingIndex2 == 3){//druga dva gena, bira x,y ili iz prvog stupca kucica
            child2.genome[mutatingIndex2] = Gene(randomInt(0,4),randomInt(0,4));

        }else if(mutatingIndex2 == 4 || mutatingIndex2 == 5){//treca dva gena, bira iz prva dva stupca kucica
            child2.genome[mutatingIndex2] = Gene(randomInt(2,6),randomInt(2,6));

        }else if (mutatingIndex2 == 6){//output, bira iz druga dva stupca kucica
            int outPut = randomInt(4,7);
            child2.genome[mutatingIndex2] = Gene(outPut,outPut);
        }
    }

    NewPop.entityList[index] = child1;
    index++;

    if(index != Constants::POPULATION_SIZE) {
        NewPop.entityList[index] = child2;
    }

    return index;
}
