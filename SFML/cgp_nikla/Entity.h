//
// Created by <3 on 07.01.2025..
//

#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <string>
#include <sstream>
#include "Gene.h"
#include "Constants.h"
using namespace std;

class Entity {

public:

    vector<Gene> genome;
    float fitness;
    float lengthTraversed;

    Entity();

    float getFitness();
    void createGenome();

    double entityFunction(vector<double> &cgpInputs);

    string toString();
    Entity stringToEntity(vector<string> splicedString);



};


#endif //ENTITY_H
