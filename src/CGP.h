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
#define MAX_MAP_SIZE 250
// broj operanada za nodeove
#define NUM_OPERANDS 7
// neki nebitan broj i guess
#define OUT_VALUE 1
// koliko daleko ispred sebe pticica vidi
#define VIEW_DISTANCE 13

// broj generacija
#define GENERATIONS 5000
// broj redova nodova
#define ROWS 10
// broj stupaca nodova
#define COLUMNS 10
// koliko levela iza se moze spojiti node (ne radi!!!)
#define LEVELS_BACK 7
// broj inputova
#define INPUTS 3
// broj outputova
#define OUTPUTS 1
// broj mutacije genoma po jedinki
#define MUTATIONS 6
// mijenja li se mapa tokom ucenja
#define SWITCH_MAP false

class CGP {
public:
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
