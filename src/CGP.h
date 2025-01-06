#ifndef CGP_H
#define CGP_H
#define TYPE double

#include <vector>
#include <string>
#include "Individual.h"
#include "Node.h"
#include "Map.h"

// max duljina mape
// o ovom ovisi fitness
const int MAX_MAP_SIZE = 250;
// broj operanada za nodeove
const int NUM_OPERANDS = 8;
// neki nebitan broj i guess
const int OUT_VALUE = 0;
// koliko daleko ispred sebe pticica vidi
const int VIEW_DISTANCE = 13;

// broj generacija
const int GENERATIONS = 10000;
// broj redova nodova
const int ROWS = 20;
// broj stupaca nodova
const int COLUMNS = 20;
// koliko levela iza se moze spojiti node
const int LEVELS_BACK = 2;
// broj inputova
const int INPUTS = 3;
// broj outputova
const int OUTPUTS = 1;
// broj mutacije genoma po jedinki
const int MUTATIONS = 6;
// broj jedinki u generaciji
const int POPULATION = 8;
// koliko cesto se mijenja mapa tokom ucenja
const int MAP_CHANGE = 200;

class CGP {
public:
    int actualGens;

    CGP();
    CGP(std::string map, int mapNum);

    void switchMap();

    std::vector<Individual> generatePopulation(int rows, int columns, int levelsBack, int inputs, int outputs);

    std::vector<Individual> mutate(int numMut, Individual parent);

    std::vector<Individual> mutate(Individual parent);

    Individual runCGP(int generations, int rows, int columns, int levelsBack, int inputs, int outputs, int mutations);
private:
    Map map;
};

#endif
