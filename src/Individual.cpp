#include "Individual.h"
#include "Simulator.h"
#include "Controller.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

TYPE computeNode(int operand, TYPE value1, TYPE value2) {
    switch (operand) {
    case 1:
        return value1 + value2;
    case 2:
        return value1 - value2;
    case 3:
        return value1 * value2;
    case 4:
        return (value2 == 0) ? 0 : value1 / value2;
    case 5:
        return sin(value1);
    case 6:
        return cos(value1);
    case 7:
        return value1 * value1;
    default:
        return 0;
    }
}

Individual::Individual() {}

Individual::Individual(vector<Node> genes, vector<Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs) {
    this->genes = genes;
    this->outputGene = outputGene;
    this->rows = rows;
    this->columns = columns;
    this->levelsBack = levelsBack;
    this->inputs = inputs;
    this->outputs = outputs;
    this->evalDone = false;
    this->fitness = 1;
}

Individual::Individual(vector<Node> genes, vector<Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs, bool evalDone, int fitness) {
    this->genes = genes;
    this->outputGene = outputGene;
    this->rows = rows;
    this->columns = columns;
    this->levelsBack = levelsBack;
    this->inputs = inputs;
    this->outputs = outputs;
    this->evalDone = evalDone;
    this->fitness = fitness;
}

void Individual::printNodes() {
    for (size_t i = 0; i < rows * columns + inputs; i++)
        cout << i << " " << genes[i].operand << " " << genes[i].connection1 << " " << genes[i].connection2 << endl;

    for (size_t j = 0; j < outputs; j++)
        cout << outputGene[j].connection << " ";

    cout << endl << endl;
}

void Individual::evaluateUsed() {
    for (size_t i = 0; i < outputs; i++)
        genes[outputGene[i].connection].used = true;


    for (int j = columns * rows + inputs - 1; j >= (columns - 1) * rows + inputs - 1; j--)
    {
        for (int k = j; k >= inputs; k = k - columns)
        {
            if (genes[k].used) {
                genes[genes[k].connection1].used = true;

                int conn2 = genes[k].connection2;
                if (conn2 > -1)
                    genes[conn2].used = true;
            }
        }
    }
    evalDone = true;
}

void Individual::evaluateValue(vector<TYPE> input) {
    if (!evalDone)
        evaluateUsed();

    for (int l = 0; l < inputs; l++)
        genes[l].outValue = input[l];

    for (int j = (columns - 1) * rows + inputs - 1; j < columns * rows + inputs; j++)
    {
        for (int k = j; k >= inputs; k = k - columns)
        {
            if (genes[k].used) {
                TYPE value1 = genes[genes[k].connection1].outValue;
                TYPE value2 = genes[k].connection2 < 0 ? 0 : genes[genes[k].connection2].outValue;

                genes[k].outValue = computeNode(genes[k].operand, value1, value2);
            }
        }
    }

    for (int m = 0; m < outputs; m++)
        outputGene[m].value = genes[outputGene[m].connection].outValue;
}

TYPE Individual::calculateFitness(TYPE lenght) {
    return 1 - lenght/MAX_MAP_SIZE;
}

Individual Individual::deserialize(std::istream& is) {
    int rows, columns, levelsBack, inputs, outputs, evalDone;
    double fitness;

    is >> rows >> columns >> levelsBack >> inputs >> outputs >> fitness >> evalDone;

    size_t genesSize;
    is >> genesSize;
    std::vector<Node> genes;
    genes.reserve(genesSize);
    for (size_t i = 0; i < genesSize; ++i) {
        Node gene;
        is >> gene;
        genes.emplace_back(gene);
    }

    size_t outputGeneSize;
    is >> outputGeneSize;
    std::vector<Output> outputGene;
    outputGene.reserve(outputGeneSize);
    for (size_t i = 0; i < outputGeneSize; ++i) {
        Output outGene;
        is >> outGene;
        outputGene.emplace_back(outGene);
    }

    return Individual(std::move(genes), std::move(outputGene), rows, columns, levelsBack, inputs, outputs, evalDone, fitness);
}