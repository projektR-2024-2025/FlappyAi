#ifndef CGP_H
#define CGP_H
#define TYPE double

#include <vector>
#include <string>
#include "CGPIndividual.h"
#include "Node.h"
#include "SelectionScreen.h"
#include "Controller.h"

// max duljina mape
// o ovom ovisi fitness
const int MAX_MAP_SIZE = 10000;
// broj operanada za nodeove
const int NUM_OPERANDS = 9;
// koliko daleko ispred sebe pticica vidi
const int VIEW_DISTANCE = 13;

// broj generacija
const int GENERATIONS = 50000;
// broj redova nodova
const int ROWS = 20;
// broj stupaca nodova
const int COLUMNS = 20;
// koliko levela iza se moze spojiti node
const int LEVELS_BACK = 3;
// broj inputova
const int INPUTS = 6;
// broj outputova
const int OUTPUTS = 1;
// broj mutacije genoma po jedinki
const int MUTATIONS = 6;
// broj jedinki u generaciji
const int POPULATION = 10;

class CGP {
private:
    int generations, rows, columns, levelsBack, inputs, outputs, mutations;
public:
    int actualGens;
    CGPIndividual bestOne;

    CGP(int generations, int rows, int columns, int levelsBack, int inputs, int outputs, int mutations)
        : generations(generations), rows(rows), columns(columns), levelsBack(levelsBack), inputs(inputs), outputs(outputs), mutations(mutations) {};

    std::vector<CGPIndividual> generatePopulation(int rows, int columns, int levelsBack, int inputs, int outputs);

    std::vector<CGPIndividual> mutate(int numMut, CGPIndividual parent);

    std::vector<CGPIndividual> mutate(CGPIndividual parent);

    CGPIndividual runCGP();

    CGPController* CGPMain(ActionType action);
};

#endif
