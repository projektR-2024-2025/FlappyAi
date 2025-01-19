//
// Created by <3 on 19.01.2025..
//

//
// Created by <3 on 07.01.2025..
//

#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <sstream>
#include "Constants.h"
#include "Gene.h"
#include "Entity.h"
using namespace std;

Entity::Entity() {
    this->fitness = 0.0f;
    this->lengthTraversed = 0.0f;
    this->genome.reserve(Constants::ENTITY_SIZE);
    this->genome.resize(Constants::ENTITY_SIZE);

}

float Entity::getFitness() {
    return fitness;
}


void Entity::createGenome() {
    for(int i = 0; i < Constants::ENTITY_SIZE; i++) {
        if(i == 0 || i == 1) {
            Gene geneSet(randomInt(0, Constants::AMOUNT_OF_CGP_INPUTS), randomInt(0,Constants::AMOUNT_OF_CGP_INPUTS));
            //genome.push_back(geneSet);
            genome[i] = geneSet;
        }else if(i == 2 || i == 3) {
            Gene geneSet(randomInt(0, Constants::AMOUNT_OF_CGP_INPUTS + 2), randomInt(0,Constants::AMOUNT_OF_CGP_INPUTS + 2));
            //genome.push_back(geneSet);
            genome[i] = geneSet;
        }else if(i == 4 || i ==5) {
            Gene geneSet(randomInt(Constants::AMOUNT_OF_CGP_INPUTS, Constants::AMOUNT_OF_CGP_INPUTS + 4), randomInt(Constants::AMOUNT_OF_CGP_INPUTS,Constants::AMOUNT_OF_CGP_INPUTS + 4));
            //genome.push_back(geneSet);
            genome[i] = geneSet;
        }else if(i == 6) {
            int output = randomInt(Constants::AMOUNT_OF_CGP_INPUTS + 2, Constants::AMOUNT_OF_CGP_INPUTS + 6);
            Gene geneSet(output, output);
            //genome.push_back(geneSet);
            genome[i] = geneSet;
        }
    }
}

double Entity::entityFunction(vector<double> &cgpInputs) {

    int totalSize = Constants::ENTITY_SIZE + Constants::AMOUNT_OF_CGP_INPUTS;
    vector<double> temp(totalSize, 0.0);

    for(int i = 0; i < cgpInputs.size(); i++){
        temp[i] = cgpInputs[i];
    }

    for(int i = cgpInputs.size(); i < Constants::ENTITY_SIZE + cgpInputs.size(); i++) {

        Gene currentGene = this->genome[i - cgpInputs.size()];
        int in1 = currentGene.in1;      //get(i - cgpInputs.size()).in1;
        int in2 = currentGene.in2;

        double result = 0;

        double a = temp[in1];
        double b = temp[in2];


        // PLUS, MINUS, MUL, DIV, MOD, COS, SIN, POW;
        switch (currentGene.function) {
            case Functions::PLUS:
                result = a + b;
                break;
            case Functions::MINUS:
                result = a - b;
                break;
            case Functions::MUL:
                result = a * b;
                break;
            case Functions::DIV:
                if(b != 0) {
                    result = a / b;
                }else {
                    result = 0;
                }
                break;
            case Functions::MOD:
                if(b != 0) {
                    result = fmod(a,b);
                }else {
                    result = 0;
                }
                break;
            case Functions::COS:
                result = cos(a + b);
                break;
            case Functions::SIN:
                result = sin(a + b);
                break;
            case Functions::POW:
                result = pow(a, b);
                break;
        }
        temp[i] = result;
    }

    int outputIndex = this->genome[Constants::ENTITY_SIZE - 1].in1;

    return temp[outputIndex];

}


string Entity::toString() {

    string output = "[";

    for(auto &gene : genome) {
        output = output + to_string(gene.in1) + to_string(gene.in2);

        switch (gene.function) {
            case Functions::PLUS:
                output = output + "PLUS ";
                break;
            case Functions::MINUS:
                output = output + "MINUS ";
                break;
            case Functions::MUL:
                output = output + "MUL ";
                break;
            case Functions::DIV:
                output = output + "DIV ";
                break;
            case Functions::MOD:
                output = output + "MOD ";
                break;
            case Functions::COS:
                output = output + "COS ";
                break;
            case Functions::SIN:
                output = output + "SIN ";
                break;
            case Functions::POW:
                output = output + "POW ";
                break;
        }
    }

    output = output + "] " + to_string(this->fitness);

return output;
}

