#ifndef FUNCTION_BINARY_TREE_H
#define FUNCTION_BINARY_TREE_H

#include <vector>
#include <string>
#include "../Parameters.h"


using namespace std;

namespace tonka{
class Node {
public:
    string value;
    int type; // 0 - funkcija s jednim argumentom, 1 - funkcija s 2 argumenta, 2 - konstanta, 3 - varijabla
    Node* left;
    Node* right;

    Node(string val, int typ);
    Node* copy() ; 
};
}


class FunctionBinaryTree {
public:
    // Deklaracije funkcija i članova
    tonka::Node* root;
    int dim;
    float fit ;
    vector<string> variable_names ={};

    FunctionBinaryTree();
    FunctionBinaryTree(int max_dubina, int dimenzija);
    
    void printTree();
    void printTree(tonka::Node* node) ;
    string toString() ;
    string toString(tonka::Node* node) ;
    float izracunaj(vector<float> ulaz);
    tonka::Node* createRandomNode(int max_dubina, int dubina);
    string generateRandomValue(int type);
    float izracunajR(tonka::Node* node, vector<float> ulaz);
    int countNodes(tonka::Node* node) ;
    bool operator==(const FunctionBinaryTree other) ;
    bool areNodesEqual(tonka::Node* node, tonka::Node* other) ;
    void fitness() ;
    
};

float funkcija0(string fja, float x) ;
float funkcija1(string fja, float x, float y) ;


#endif