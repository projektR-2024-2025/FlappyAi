#ifndef FUNCTION_BINARY_TREE_H
#define FUNCTION_BINARY_TREE_H

#include <vector>
#include <string>
#include "../Parameters.h"


int myrand();

using namespace std;

namespace tonka{
class NodeT {
public:
    string value;
    int type; // 0 - funkcija s jednim argumentom, 1 - funkcija s 2 argumenta, 2 - konstanta, 3 - varijabla
    NodeT* left;
    NodeT* right;

    NodeT(string val, int typ);
    NodeT* copy() ;
};
}


class FunctionBinaryTree {
public:
    // Deklaracije funkcija i članova
    tonka::NodeT* root;
    int dim;
    float fit ;
    vector<string> variable_names ={};

    FunctionBinaryTree();
    FunctionBinaryTree(int max_dubina, int dimenzija);
    
    void printTree();
    void printTree(tonka::NodeT* node) ;
    string toString() ;
    string toString(tonka::NodeT* node) ;
    float izracunaj(vector<float> ulaz);
    tonka::NodeT* createRandomNode(int max_dubina, int dubina);
    string generateRandomValue(int type);
    float izracunajR(tonka::NodeT* node, vector<float> ulaz);
    int countNodes(tonka::NodeT* node) ;
    bool operator==(const FunctionBinaryTree other) ;
    bool areNodesEqual(tonka::NodeT* node, tonka::NodeT* other) ;
    void fitness() ;
    
};

float funkcija0(string fja, float x) ;
float funkcija1(string fja, float x, float y) ;


#endif