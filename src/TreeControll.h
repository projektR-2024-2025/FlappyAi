#ifndef TREECONTROLL_H  // Include guard
#define TREECONTROLL_H

#include <string>
#include "Simulator.h"
#include "Agent.h"
// Array of operator strings used in Node class for printing
extern std::string ops[];

// Node class declaration
class Node {
public:
    int value;
    Node* left;
    Node* right;

    Node(int initialValue = 0);
    Node* deepNodeCopy();
    Node* parseNodePrefix(std::istringstream& stream);
    void printNode();
    std::string nodePrefix();
    ~Node();
};

// Tree class declaration
class Tree {
public:
    Node* root;
    double fitness = 0;

    Tree(int x = 0);
    Tree(Node* rootNode);
    void printTree();
    std::string treePrefix();
    void parseTreePrefix(const std::string& prefix);
    ~Tree();
};

#endif  // TREECONTROLL_H
