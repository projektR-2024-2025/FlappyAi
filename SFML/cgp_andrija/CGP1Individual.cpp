#include "CGP1Individual.h"
#include "../Simulator.h"
#include "../Controller.h"
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

CGP1Individual::CGP1Individual() {
}

CGP1Individual::CGP1Individual(vector<CGP1Node> genes, vector<CGP1Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs) {
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

CGP1Individual::CGP1Individual(vector<CGP1Node> genes, vector<CGP1Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs, bool evalDone) {
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

void CGP1Individual::printNodes() {
    for (size_t i = 0; i < rows * columns + inputs; i++)
        cout << i << " " << genes[i].operand << " " << genes[i].connection1 << " " << genes[i].connection2 << endl;

    for (size_t j = 0; j < outputs; j++)
        cout << outputGene[j].connection << " ";

    cout << endl << endl;
}

void CGP1Individual::printFuction() {
    cout << "Funkcija: " << evalFunction(outputGene[0].connection) << endl;
}

std::string CGP1Individual::evalFunction(int CGP1NodeNum) {
    std::ostringstream oss;

    if (CGP1NodeNum < inputs) {
        switch (CGP1NodeNum) {
        case 0:
            oss << "pipe.x";
            return oss.str();
        case 1:
            oss << "pipe.bottomY";
            return oss.str();
        case 2:
            oss << "pipe.topY";
            return oss.str();
        case 3:
            oss << "obstacleAhead";
            return oss.str();
        case 4:
            oss << "bird.position";
            return oss.str();
        case 5:
            oss << "bird.velocity";
            return oss.str();
        }
    }

    switch (genes[CGP1NodeNum].operand) {
    case 1:
        oss << "(" << evalFunction(genes[CGP1NodeNum].connection1) << " + " << evalFunction(genes[CGP1NodeNum].connection2) << ")";
        return oss.str();
    case 2:
        oss << "(" << evalFunction(genes[CGP1NodeNum].connection1) << " - " << evalFunction(genes[CGP1NodeNum].connection2) << ")";
        return oss.str();
    case 3:
        oss << "(" << evalFunction(genes[CGP1NodeNum].connection1) << " * " << evalFunction(genes[CGP1NodeNum].connection2) << ")";
        return oss.str();
    case 4:
        oss << "(" << evalFunction(genes[CGP1NodeNum].connection1) << " / " << evalFunction(genes[CGP1NodeNum].connection2) << ")";
        return oss.str();
    case 5:
        oss << "sin(" << evalFunction(genes[CGP1NodeNum].connection1) << ")";
        return oss.str();
    case 6:
        oss << "cos(" << evalFunction(genes[CGP1NodeNum].connection1) << ")";
        return oss.str();
    case 7:
        oss << "sqrt(" << evalFunction(genes[CGP1NodeNum].connection1) << ")";
        return oss.str();
    case 8:
        oss << evalFunction(genes[CGP1NodeNum].connection1) << "^2";
        return oss.str();
    case 9:
        oss << "2^" << evalFunction(genes[CGP1NodeNum].connection1);
        return oss.str();
    }

    return "";
}

void CGP1Individual::evaluateUsed() {
    isUsed(outputGene[0].connection);

    evalDone = true;
}

void CGP1Individual::isUsed(int CGP1NodeNum) {
    genes[CGP1NodeNum].used = true;

    if (genes[CGP1NodeNum].connection1 >= 0)
        isUsed(genes[CGP1NodeNum].connection1);

    if (genes[CGP1NodeNum].connection2 >= 0)
        isUsed(genes[CGP1NodeNum].connection2);
}

void CGP1Individual::evaluateValue(vector<TYPE> input) {
    clearInd();

    for (int l = 0; l < inputs; l++)
        genes[l].outValue = input[l];

    for (int m = 0; m < outputs; m++)
        outputGene[m].value = evalNode(outputGene[m].connection);
}

TYPE CGP1Individual::evalNode(int CGP1NodeNum) {

    if (isnan(genes[CGP1NodeNum].outValue)) {
        TYPE value1 = evalNode(genes[CGP1NodeNum].connection1);
        TYPE value2 = genes[CGP1NodeNum].connection2 < 0 ? 0 : evalNode(genes[CGP1NodeNum].connection2);

        genes[CGP1NodeNum].outValue = computeNode(genes[CGP1NodeNum].operand, value1, value2);
    }
    
    return genes[CGP1NodeNum].outValue;
}

void CGP1Individual::clearInd() {
    for (int i = inputs; i < genes.size(); i++)
        genes[i].outValue = NAN;
}

TYPE CGP1Individual::calculateFitness(TYPE lenght) {
    return lenght;
}

CGP1Individual CGP1Individual::deserialize(std::istream& is) {
    int rows, columns, levelsBack, inputs, outputs, evalDone;

    is >> rows >> columns >> levelsBack >> inputs >> outputs >> evalDone;

    size_t genesSize;
    is >> genesSize;
    std::vector<CGP1Node> genes;
    genes.reserve(genesSize);
    for (size_t i = 0; i < genesSize; ++i) {
        CGP1Node gene;
        is >> gene;
        genes.emplace_back(gene);
        }

    size_t outputGeneSize;
    is >> outputGeneSize;
    std::vector<CGP1Output> outputGene;
    outputGene.reserve(outputGeneSize);
    for (size_t i = 0; i < outputGeneSize; ++i) {
        CGP1Output outGene;
        is >> outGene;
        outputGene.emplace_back(outGene);
    }

    return CGP1Individual(std::move(genes), std::move(outputGene), rows, columns, levelsBack, inputs, outputs, evalDone);
}

bool CGP1Individual::findLoops(int CGP1NodeNum, std::vector<int> CGP1NodeSet) {
    branches.clear();

    return loopFinder(CGP1NodeNum, CGP1NodeSet);;
}

bool CGP1Individual::loopFinder(int CGP1NodeNum, std::vector<int> CGP1NodeSet) {

    for (int i = 0; i < CGP1NodeSet.size(); i++)
        if (CGP1NodeSet[i] == CGP1NodeNum) {
            CGP1NodeSet.push_back(CGP1NodeNum);
            branches.push_back(CGP1NodeSet);
            return true;
        }

    CGP1NodeSet.push_back(CGP1NodeNum);

    if (CGP1NodeNum < inputs) {
        return false;
    }

    bool conn1 = loopFinder(genes[CGP1NodeNum].connection1, CGP1NodeSet);
    bool conn2 = genes[CGP1NodeNum].connection2 == -1 ? false : loopFinder(genes[CGP1NodeNum].connection2, CGP1NodeSet);

    return conn1 || conn2;
}

void CGP1Individual::resolveLoops() {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> connectionDis(0, genes.size() - 1);

    vector<int> CGP1NodeSet;

    while (findLoops(outputGene[0].connection, CGP1NodeSet)) {
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

        CGP1NodeSet.clear();
    }
}