#include "CGPIndividual.h"
#include "Simulator.h"
#include "Controller.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <omp.h>

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
        return sqrt(value1);
    case 8:
        return pow(value1, 2);
    case 9:
        return pow(2, value1);
    default:
        return 0;
    }
}

CGPIndividual::CGPIndividual() {
}

CGPIndividual::CGPIndividual(vector<Node> genes, vector<Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs) {
    vector<vector<int>> branches;
    this->branches = branches;
    this->genes = genes;
    this->outputGene = outputGene;
    this->rows = rows;
    this->columns = columns;
    this->levelsBack = levelsBack;
    this->inputs = inputs;
    this->outputs = outputs;
    this->evalDone = false;
}

CGPIndividual::CGPIndividual(vector<Node> genes, vector<Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs, bool evalDone) {
    vector<vector<int>> branches;
    this->branches = branches;
    this->genes = genes;
    this->outputGene = outputGene;
    this->rows = rows;
    this->columns = columns;
    this->levelsBack = levelsBack;
    this->inputs = inputs;
    this->outputs = outputs;
    this->evalDone = evalDone;
}

void CGPIndividual::printNodes() {
    for (size_t i = 0; i < rows * columns + inputs; i++)
        cout << i << " " << genes[i].operand << " " << genes[i].connection1 << " " << genes[i].connection2 << endl;

    for (size_t j = 0; j < outputs; j++)
        cout << outputGene[j].connection << " ";

    cout << endl << endl;
}

void CGPIndividual::evaluateUsed() {
    isUsed(outputGene[0].connection);

    evalDone = true;
}

void CGPIndividual::isUsed(int nodeNum) {
    genes[nodeNum].used = true;

    if (genes[nodeNum].connection1 >= 0)
        isUsed(genes[nodeNum].connection1);

    if (genes[nodeNum].connection2 >= 0)
        isUsed(genes[nodeNum].connection2);
}

void CGPIndividual::evaluateValue(vector<TYPE> input) {
    clearInd();

    for (int l = 0; l < inputs; l++)
        genes[l].outValue = input[l];

    for (int m = 0; m < outputs; m++)
        outputGene[m].value = evalNode(outputGene[m].connection);
}

TYPE CGPIndividual::evalNode(int nodeNum) {

    if (isnan(genes[nodeNum].outValue)) {
        TYPE value1 = evalNode(genes[nodeNum].connection1);
        TYPE value2 = genes[nodeNum].connection2 < 0 ? 0 : evalNode(genes[nodeNum].connection2);

        genes[nodeNum].outValue = computeNode(genes[nodeNum].operand, value1, value2);
    }
    
    return genes[nodeNum].outValue;
}

void CGPIndividual::clearInd() {
    for (int i = inputs; i < genes.size(); i++)
        genes[i].outValue = NAN;
}

TYPE CGPIndividual::calculateFitness(TYPE lenght) {
    return lenght;
}

CGPIndividual CGPIndividual::deserialize(std::istream& is) {
    int rows, columns, levelsBack, inputs, outputs, evalDone;

    is >> rows >> columns >> levelsBack >> inputs >> outputs >> evalDone;

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

    return CGPIndividual(std::move(genes), std::move(outputGene), rows, columns, levelsBack, inputs, outputs, evalDone);
}

bool CGPIndividual::findLoops(int nodeNum, std::vector<int> nodeSet) {
    branches.clear();

    return loopFinder(nodeNum, nodeSet);;
}

bool CGPIndividual::loopFinder(int nodeNum, std::vector<int> nodeSet) {

    for (int i = 0; i < nodeSet.size(); i++)
        if (nodeSet[i] == nodeNum) {
            nodeSet.push_back(nodeNum);
            branches.push_back(nodeSet);
            return true;
        }

    nodeSet.push_back(nodeNum);

    if (nodeNum < inputs) {
        return false;
    }

    bool conn1 = loopFinder(genes[nodeNum].connection1, nodeSet);
    bool conn2 = genes[nodeNum].connection2 == -1 ? false : loopFinder(genes[nodeNum].connection2, nodeSet);

    return conn1 || conn2;
}

void CGPIndividual::resolveLoops() {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> connectionDis(0, genes.size() - 1);

    vector<int> nodeSet;

    while (findLoops(outputGene[0].connection, nodeSet)) {
        for (int i = 0; i < branches.size(); i++) {
            int cell1 = branches[i][branches[i].size() - 2];
            int cell2 = branches[i][branches[i].size() - 1];

            if (genes[cell1].connection1 == cell2) {
                genes[cell1].connection1 = connectionDis(gen);

                while (true) {
                    if (genes[cell1].connection1 < inputs)
                        break;
                    if ((genes[cell1].connection1 % columns) == (cell1 % columns))
                        genes[cell1].connection1 = connectionDis(gen);
                    else if (((genes[cell1].connection1 - inputs) % columns) > (((cell1 - inputs) % columns) + levelsBack))
                        genes[cell1].connection1 = connectionDis(gen);
                    else
                        break;
                }
            }
            else if (genes[cell1].connection2 == cell2) {
                genes[cell1].connection2 = connectionDis(gen);

                while (true) {
                    if (genes[cell1].connection2 < inputs)
                        break;
                    if ((genes[cell1].connection2 % columns) == (cell1 % columns))
                        genes[cell1].connection2 = connectionDis(gen);
                    else if (((genes[cell1].connection2 - inputs) % columns) > (((cell1 - inputs) % columns) + levelsBack))
                        genes[cell1].connection2 = connectionDis(gen);
                    else
                        break;
                }
            }
        }

        nodeSet.clear();
    }
}