#ifndef TREE_PARSER_H
#define TREE_PARSER_H

#include "FunctionBinaryTree.h"
#include <string>
#include <vector>

using namespace std;

class TreeParser {
public:
    static FunctionBinaryTree fromString(const string& str, int dimension);

private:
    static tonka::Node* parseExpression(const string& str, size_t& pos);
    static string parseToken(const string& str, size_t& pos);
    static vector<string> functions0; // Type 0 functions
    static vector<string> functions1; // Type 1 functions
};

#endif
