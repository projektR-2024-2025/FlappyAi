#include "TreeParser.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

vector<string> TreeParser::functions0 = {"aps", "sin", "cos"};
vector<string> TreeParser::functions1 = {"+", "-", "*", "/"};

FunctionBinaryTree TreeParser::fromString(const string& str, int dimension) {
    FunctionBinaryTree tree;
    tree.dim = dimension;
    size_t pos = 0;
    tree.root = parseExpression(str, pos);
    return tree;
}

tonka::NodeT* TreeParser::parseExpression(const string& str, size_t& pos) {
    // Skip whitespace
    while (pos < str.length() && isspace(str[pos])) pos++;
    
    if (pos >= str.length()) return nullptr;
    
    // Handle parenthesized expressions
    if (str[pos] == '(') {
        pos++; // Skip '('
        
        // Parse left expression
        tonka::NodeT* left = parseExpression(str, pos);
        
        // Get operator
        string op = parseToken(str, pos);
        tonka::NodeT* node = new tonka::NodeT(op, 1); // Type 1 for binary operator
        node->left = left;
        
        // Parse right expression
        node->right = parseExpression(str, pos);
        
        // Skip closing parenthesis
        while (pos < str.length() && str[pos] != ')') pos++;
        if (pos < str.length()) pos++; // Skip ')'
        
        return node;
    }
    
    // Handle functions (e.g., sin(x))
    string token = parseToken(str, pos);
    if (find(functions0.begin(), functions0.end(), token) != functions0.end()) {
        tonka::NodeT* node = new tonka::NodeT(token, 0); // Type 0 for unary function
        
        // Skip opening parenthesis
        while (pos < str.length() && str[pos] != '(') pos++;
        if (pos < str.length()) pos++; // Skip '('
        
        // Parse argument
        node->left = parseExpression(str, pos);
        
        // Skip closing parenthesis
        while (pos < str.length() && str[pos] != ')') pos++;
        if (pos < str.length()) pos++; // Skip ')'
        
        return node;
    }
    
    // Handle variables (x0, x1, etc.)
    if (token[0] == 'x') {
        string index = token.substr(1); // Remove 'x' prefix
        return new tonka::NodeT(index, 3); // Type 3 for variable
    }
    
    // Handle constants
    try {
        stof(token); // Verify it's a valid float
        return new tonka::NodeT(token, 2); // Type 2 for constant
    } catch (...) {
        throw runtime_error("Invalid token: " + token);
    }
}

string TreeParser::parseToken(const string& str, size_t& pos) {
    // Skip whitespace
    while (pos < str.length() && isspace(str[pos])) pos++;
    
    string token;
    
    // Handle variables (x0, x1, etc.)
    if (pos < str.length() && str[pos] == 'x') {
        token += str[pos++];
        while (pos < str.length() && isdigit(str[pos])) {
            token += str[pos++];
        }
        return token;
    }
    
    // Handle operators, functions, and constants
    while (pos < str.length() && !isspace(str[pos]) && 
           str[pos] != '(' && str[pos] != ')') {
        token += str[pos++];
    }
    
    return token;
}