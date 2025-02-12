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
#include <iostream>
#include <fstream>
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

Entity::~Entity() {}


float Entity::getFitness() {
    return fitness;
}


void Entity::createGenome() {
    int a;
    int b;
    //int amountOfCgpInputsMin = Constants::AMOUNT_OF_CGP_INPUTS;
   // int amountOfCgpInputsMax = Constants::AMOUNT_OF_CGP_INPUTS;
    for(int i = 0; i < Constants::ENTITY_SIZE; i++) {
        // preprsvi da i moze ici dalje
        if(i == 0 || i == 1) { // prvi stupac
            a = randomInt(0, Constants::AMOUNT_OF_CGP_INPUTS - 1);
            b = randomInt(0,Constants::AMOUNT_OF_CGP_INPUTS - 1);
            Gene geneSet(a , b);
            //genome.push_back(geneSet);
            genome[i] = geneSet;
        }else if(i == 2 || i == 3) { // drugi stupac
            a = randomInt(0, Constants::AMOUNT_OF_CGP_INPUTS + 1);
            b = randomInt(0,Constants::AMOUNT_OF_CGP_INPUTS + 1);
            Gene geneSet(a , b);
            //genome.push_back(geneSet);
            genome[i] = geneSet;
        }else if(i >= 4 && i < Constants::ENTITY_SIZE - 1) {
            if(i % 2 == 0) {
                a = randomInt(i, i + 3);
                b = randomInt(i ,i + 3);

            }else if (i % 2 == 1) {
                a = randomInt(i - 1, i + 2);
                b = randomInt(i - 1 ,i + 2);
            }
            Gene geneSet(a , b);
            genome[i] = geneSet;
        }


        if(i == Constants::ENTITY_SIZE - 1) {
            int output = randomInt(Constants::ENTITY_SIZE - 3, Constants::ENTITY_SIZE - 2);//bilo je 6 prije
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

    string output;

    for(auto &gene : genome) {
        output = output + to_string(gene.in1) + " " + to_string(gene.in2) + " ";

        switch (gene.function) {
            case Functions::PLUS:
                //output = output + "PLUS ";
                output = output + "1/";
                break;
            case Functions::MINUS:
                //output = output + "MINUS ";
                output = output + "2/";
                break;
            case Functions::MUL:
                //output = output + "MUL ";
                output = output + "3/";
                break;
            case Functions::DIV:
                //output = output + "DIV ";
                output = output + "4/";
                break;
            case Functions::MOD:
                //output = output + "MOD ";
                output = output + "5/";
                break;
            case Functions::COS:
                //output = output + "COS ";
                output = output + "6/";
                break;
            case Functions::SIN:
                //output = output + "SIN ";
                output = output + "7/";
                break;
            case Functions::POW:
                //output = output + "POW ";
                output = output + "8/";
                break;
        }
    }

    output = output + to_string(this->fitness);

return output;
}

Entity Entity::stringToEntity(string filePath) {

    Entity entity = Entity();
    fstream myFile; // file iz kojeg se cita
    string fileText;
    stringstream textStream;
    string gene;
    string geneSegment;
    stringstream geneStream;

    myFile.open (filePath);

    if (myFile.is_open())
    {
        getline(myFile, fileText);
        myFile.close();
    }else {
        cout << "error opening file(Controller.h)\n";
    }

    textStream = (stringstream)fileText;

    vector<vector<string>> listOfSplicedGenes;
    vector<string> splicedGenes;

    while(std::getline(textStream, gene, '/')) {
        geneStream = (stringstream)gene;
        while(std::getline(geneStream, geneSegment, ' ')) {
            splicedGenes.push_back(geneSegment);
        }
        listOfSplicedGenes.push_back(splicedGenes);
        splicedGenes.clear();
    }



    for(int i = 0; i < Constants::ENTITY_SIZE + 1; i++) {

        if(i < Constants::ENTITY_SIZE) {
            entity.genome[i].in1 = stoi(listOfSplicedGenes[i][0]);
            entity.genome[i].in2 = stoi(listOfSplicedGenes[i][1]);

            switch(stoi(listOfSplicedGenes[i][2])) {

                case 1:
                    entity.genome[i].function = Functions::PLUS;
                break;

                case 2:
                    entity.genome[i].function = Functions::MINUS;
                break;

                case 3:
                    entity.genome[i].function = Functions::MUL;
                break;

                case 4:
                    entity.genome[i].function = Functions::DIV;
                break;

                case 5:
                    entity.genome[i].function = Functions::MOD;
                break;

                case 6:
                    entity.genome[i].function = Functions::COS;
                break;

                case 7:
                    entity.genome[i].function = Functions::SIN;
                break;

                case 8:
                    entity.genome[i].function = Functions::POW;
                break;


            }

        }else if(i == Constants::ENTITY_SIZE) {
            entity.fitness = stof(listOfSplicedGenes[i][0]);
        }

    }


return entity;
}


