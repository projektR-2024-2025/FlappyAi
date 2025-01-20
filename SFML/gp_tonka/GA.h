#ifndef GA_H
#define GA_H

#include <vector>
#include "FunctionBinaryTree.h"
#include "../SelectionScreen.h"


using namespace std;
using namespace tonka;

struct GAParameters {
    float mutation_rate;
    int k;
    int penalty;
    int br;
    float elite_percentage;
    float tooBad;
    int max_depth;

    void adapt(float diversity, float best_fitness);
};

// Function declarations
void resetMutation(FunctionBinaryTree& jedinka);
float calculate_diversity(const vector<FunctionBinaryTree>& population);
vector<FunctionBinaryTree> stvori_populaciju(int dim, int vel, int max_dubina);
float penalty(string f, int dim);
void evaluacija_jedinke(FunctionBinaryTree& jedinka, FunctionBinaryTree& najbolja, int dim);
void evaluacija_populacije(vector<FunctionBinaryTree>& populacija, FunctionBinaryTree& najbolja, int dim);
vector<int> ksel(vector<FunctionBinaryTree>& p, int k);
void preserveElites(vector<FunctionBinaryTree>& population, int eliteCount);
FunctionBinaryTree krizanje(FunctionBinaryTree r1, FunctionBinaryTree r2);
tonka::Node* changeRandomNode(tonka::Node* node, FunctionBinaryTree jed, int depth);
void mutacija(FunctionBinaryTree& jedinka);
void obrisi(vector<FunctionBinaryTree>& p, int r3);
FunctionBinaryTree ga(int dim, int vel, int max_dubina, int max_ev);
Controller* GPMain(ActionType action);
void saveBestToFile(FunctionBinaryTree best);
FunctionBinaryTree loadBestFromFile() ;


#endif // GA_H