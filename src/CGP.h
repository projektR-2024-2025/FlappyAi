#ifndef CGP_H
#define CGP_H
#define TYPE double

#include <vector>
#include <string>
#include "Individual.h"
#include "Node.h"

// max duljina mape
// o ovom ovisi fitness
#define MAX_MAP_SIZE 150
// broj operanada za nodeove
#define NUM_OPERANDS 7
// neki nebitan broj i guess
#define OUT_VALUE 0
// koliko daleko ispred sebe pticica vidi
#define VIEW_DISTANCE 12

// broj generacija
#define GENERATIONS 5000
// broj redova nodova
#define ROWS 4
// broj stupaca nodova
#define COLUMNS 4
// koliko levela iza se moze spojiti node (ne radi!!!)
#define LEVELS_BACK 0
// broj inputova
#define INPUTS 4
// broj outputova
#define OUTPUTS 1
// broj mutacije genoma po jedinki
#define MUTATIONS 4

class CGP {
public:
    CGP();
    CGP(std::string map);

    std::vector<Individual> generatePopulation(int rows, int columns, int levelsBack, int inputs, int outputs);

    std::vector<Individual> mutate(int numMut, Individual parent);

    Individual runCGP(int generations, int rows, int columns, int levelsBack, int inputs, int outputs, int mutations);
private:
    std::string map;
};

#endif
