#ifndef TREECONTROLL_H  // Include guard
#define TREECONTROLL_H

#include <string>
// Array of operator strings used in Node2 class for printing
extern std::string ops[];

// Node2 class declaration
class Node2 {
public:
    int value;
    Node2* left;
    Node2* right;

    Node2(int initialValue = 0);
    Node2* deepNodeCopy();
    Node2* parseNodePrefix(std::istringstream& stream);
    void printNode();
    std::string nodePrefix();
    ~Node2();
};

// Tree class declaration
class Tree {
public:
    Node2* root;
    double fitness = 0;

    Tree(int x = 0);
    Tree(Node2* rootNode);
    void printTree();
    std::string treePrefix();
    void parseTreePrefix(const std::string& prefix);
    ~Tree();
};

#endif  // TREECONTROLL_H
