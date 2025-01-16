#include <iostream>
#include <sstream>
#include <string>
#include "TreeControll.h"
#include "Simulator.h"
#include "Agent.h"

// Definition of ops[] array
// BINARNI 0-10
// UNARNI 11
// konst 12-18
//                    0    1    2    3    4    5     6     7    8     9     10     11    12   13      14                       15            16                 17                    18                 19
//                    A    B    C    D    E    F     G     H    I     J     K      L     M    N       O                       P                Q                R                     S                   T
std::string ops[] = {"+", "-", "*", "/", "<", ">", "<=", ">=", "=", "and", "or", "not", "0", "1", "nextPillarDist()", "aboveNextBot()", "bellowNextTop()", "currentVelocity()", "currentYPOS()", "obstacleAhead()"};

// Definition of Node class constructor
Node::Node(int initialValue) : value(initialValue), left(nullptr), right(nullptr) {}

// Definition of printNode method
void Node::printNode() {
    if (this == nullptr) return;
    if (this->left != nullptr) {
        std::cout << "(";
        this->left->printNode();
        std::cout << ") ";
    }
    std::cout << ops[value];
    if (this->right != nullptr) {
        std::cout << " (";
        this->right->printNode();
        std::cout << ")";
    }
}

std::string Node::nodePrefix() {
    std::string prefix = "";
    char c = this->value + 65;
    //std::cout << "Printing for: " << this->value << " " << c << "\n";

    prefix += c;
    if (this->left == nullptr) prefix += " @";
    else prefix += " " + this->left->nodePrefix();
    if (this->right == nullptr) prefix += " @";
    else prefix += " " + this->right->nodePrefix();

    return prefix;
}

Node* Node::deepNodeCopy(){
    if (this == nullptr) return nullptr;
    Node* coppied = new Node(this->value);
    // Copy all data.
    coppied->value = this->value;
    coppied->left = this->left->deepNodeCopy();
    coppied->right = this->right->deepNodeCopy();

    // Unary operator check.
    //if ((coppied->value >= 0 && coppied->value <= 10) && (coppied->left == nullptr)) coppied->left = new Node(12 + rand() % 7);

    return coppied;
}

// Function to parse the node prefix and create a Node tree
Node* Node::parseNodePrefix(std::istringstream& stream) {
    std::string token;
    if (!(stream >> token)) return nullptr;

    if (token == "@") return nullptr; // Null node indicator

    // Convert the token (character) back to its numerical value
    int value = token[0] - 'A';

    // Create a new Node with the value
    Node* node = new Node(value);

    // Recursively parse the left and right children
    node->left = parseNodePrefix(stream);
    node->right = parseNodePrefix(stream);

    return node;
}

// Definition of Node class destructor
Node::~Node() {
    delete left;
    delete right;
    left = nullptr;
    right = nullptr;
}

// Definition of Tree class constructor
Tree::Tree(int x) : root(new Node(x)) {}
Tree::Tree(Node* rootNode) : root(rootNode) {}

// Definition of printTree method
void Tree::printTree() {
    root->printNode();
    std::cout << "\n";
}

std::string Tree::treePrefix() {
    return root->nodePrefix();
}

// Function to parse a tree prefix string and create a Tree object
void Tree::parseTreePrefix(const std::string& prefix) {
    std::istringstream stream(prefix); // Create a stream from the string
    Node* root = root->parseNodePrefix(stream); // Parse the root node
    this->root = root;
}

// Definition of Tree class destructor
Tree::~Tree() {
    delete root;
    root = nullptr;
}
