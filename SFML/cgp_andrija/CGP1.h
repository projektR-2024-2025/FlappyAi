#ifndef CGP1_H
#define CGP1_H
#define TYPE double

#include <vector>
#include <string>
#include "CGP1Individual.h"
#include "CGP1Node.h"
#include "../SelectionScreen.h"
#include "../Controller.h"

// max duljina mape
// o ovom ovisi fitness
const int MAX_MAP_SIZE = 5000;
// broj operanada za nodeove
const int NUM_OPERANDS = 9;
// koliko daleko ispred sebe pticica vidi
const int VIEW_DISTANCE = 13;

// broj generacija
const int GENERATIONS = 5000;
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
const int POPULATION = 20;
// najbolja jedinka
const std::string bestFile = "CGP_best.txt";

class CGP1 {
private:
    int generations, rows, columns, levelsBack, inputs, outputs, mutations;
    sf::RenderWindow& window;
public:
    int actualGens;
    CGP1Individual bestOne;

    CGP1(int generations, int rows, int columns, int levelsBack, int inputs, int outputs, int mutations, sf::RenderWindow& window)
        : generations(generations), rows(rows), columns(columns), levelsBack(levelsBack), inputs(inputs), outputs(outputs), mutations(mutations), window(window) {};

    std::vector<CGP1Individual> generatePopulation(int rows, int columns, int levelsBack, int inputs, int outputs);

    std::vector<CGP1Individual> mutate(int numMut, CGP1Individual parent);

    std::vector<CGP1Individual> mutate(CGP1Individual parent);

    CGP1Individual runCGP();

    static CGP1Controller* CGPMain(sf::RenderWindow& window);
};

#endif
