#ifndef FUNCTION_BINARY_TREE_H
#define FUNCTION_BINARY_TREE_H

#include <vector>
#include <string>

using namespace std;


class Node {
public:
    string value;
    int type; // 0 - funkcija s jednim argumentom, 1 - funkcija s 2 argumenta, 2 - konstanta, 3 - varijabla
    Node* left;
    Node* right;

    Node(string val, int typ);
    Node* copy() ; 
};

class FunctionBinaryTree {
public:
    // Deklaracije funkcija i članova
    Node* root;
    int dim;
    float fit ;
    vector<string> variable_names ={};

    FunctionBinaryTree();
    FunctionBinaryTree(int max_dubina, int dimenzija);
    
    void printTree();
    void printTree(Node* node) ;
    string toString() ;
    string toString(Node* node) ;
    float izracunaj(vector<float> ulaz);
    Node* createRandomNode(int max_dubina, int dubina);
    string generateRandomValue(int type);
    float izracunajR(Node* node, vector<float> ulaz);
    int countNodes(Node* node) ;
    bool operator==(const FunctionBinaryTree other) ;
    bool areNodesEqual(Node* node, Node* other) ;
    
};

float funkcija0(string fja, float x) ;
float funkcija1(string fja, float x, float y) ;

#endif