//
// Created by <3 on 08.01.2025..
//

#ifndef POPULATION_H
#define POPULATION_H
#include <vector>
#include "Entity.h"
using namespace std;

class Population {

public:
    vector<Entity> entityList;
    Entity parent1;
    Entity parent2;

    Population();
    ~Population();

    void entityFitnessSort();
    Entity& getBestEntity();
    void chooseParents();
    void entityFitnessEval();
    int crossingOverAndMutation(Population &NewPop, int index);




};



#endif //POPULATION_H
