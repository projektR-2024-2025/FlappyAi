#include "FunctionBinaryTree.h"
#include "../Simulator.h"
#include "../Agent.h"
#include "../Controller.h"
//#include "GPcontroller.h"


#include<iostream>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<string>
#include<cmath>
#include <stdexcept>


using namespace std ;

float epsilon = 0.000000000001 ;
float replacement = 0.1 ;
const int MAX_MAP_SIZE = 8000;

vector<string> functions0 = { "aps", "sin", "cos"}; // Type 0 functions //poslije dodati trigonometrijske
vector<string> functions1 = { "+","-","*","/"}; // Type 1 functions
vector<string> functions2 = { "<", ">", "==", "&&", "||"} ;//Type 2 functions -> subset of type 1 functions

float funkcija0(string fja, float x){
    if(fja == "sin") return sin(x) ;
    if(fja == "cos") return cos(x) ;
    if(fja == "tan") return tan(x) ;
    if(fja == "aps") return abs(x) ;
    throw invalid_argument("Nepoznata funkcija: " + fja); // Izuzetak za nepoznatu funkciju

}
float funkcija1(string fja, float x, float y){
    if(fja == "+") return x + y ;
    if(fja == "-") return x - y ;
    if(fja == "*") return x * y ;
    if(fja == "/"){
        if(abs(y) < epsilon ){
            //throw invalid_argument("dijeljenje nulom");
            y = replacement ;
        }
        return x/y ;
    }
    if(fja == "**"){
        //dodaj iznimke
        if (x < 0 && y != floor(y)) {
            //throw invalid_argument("korijen iz negativnog broja");
            x = -x ;
        }
        return pow(x, y) ;
    }
    if(fja == "<") return x < y ;
    if(fja == ">") return x > y ;
    if(fja == "==") return x==y ;
    if(fja == "&&") return (x != 0)*(y!=0) ;
    if(fja == "||") return (x != 0)+(y!=0) ;
    throw invalid_argument("Nepoznata funkcija: " + fja); // Izuzetak za nepoznatu funkciju

}


tonka::NodeT::NodeT(string val, int typ) :
  value(val), type(typ), left(nullptr), right(nullptr) 
{}



FunctionBinaryTree::FunctionBinaryTree() : root(nullptr) {}

FunctionBinaryTree::FunctionBinaryTree(int max_dubina, int dimenzija){
    //srand(time(0)); // Initialize random seed
    dim = dimenzija ;
    root = createRandomNode(max_dubina, 0);
}
    
tonka::NodeT* FunctionBinaryTree::createRandomNode(int max_dubina, int dubina) {
    int type;
    string value ;
    if (dubina < max_dubina) {
        type = rand() % 4; //  0, 1, 2, 3
    } else {
        type = rand() % 2 + 2; // na kraju mogu biti samo konstante i varijable (2 ili 3)
    }
    if(dubina == 0){
        type = 1 ; //root moze biti samo funkcija
        value = functions1[rand() % functions1.size()] ; //neka root bude bool fja
    }
    else{
        value = generateRandomValue(type);
    }
    tonka::NodeT* node = new tonka::NodeT(value, type);

    if (type == 0 && dubina < max_dubina) { // funkcija jedan argument
        node->left = createRandomNode(max_dubina, dubina + 1); //dodaje se lijevo, desno ostaje null
    } else if (type == 1 && dubina < max_dubina) { // funkcija s dva argumenta
        node->left = createRandomNode(max_dubina, dubina + 1);
        node->right = createRandomNode(max_dubina, dubina + 1);
    }
    return node;
}

string FunctionBinaryTree::generateRandomValue(int type) {    
    string s = "" ;

    if (type == 0) return functions0[rand() % functions0.size()];
    if (type == 1) return functions1[rand() % functions1.size()];

    if(type == 2) return to_string( static_cast<float>(rand()) / RAND_MAX);
    if(type == 3) return to_string(rand() % dim) ; //index varijable  
    return "greska" ;     
}

void FunctionBinaryTree::printTree(tonka::NodeT* node) {
    if (node == nullptr) return;
    if(node->type == 3){
        cout << "x" + node->value <<" " ;
    }
    else{
        cout << node->value << " ";
    }
    printTree(node->left);
    printTree(node->right);
}

void FunctionBinaryTree::printTree() {
        printTree(root);
        cout <<endl ;
    }

float FunctionBinaryTree::izracunaj(vector<float> ulaz){
        return (izracunajR(root, ulaz)) ;
    }
float FunctionBinaryTree::izracunajR(tonka::NodeT *node, vector<float> ulaz){
        if(node->type == 2){ //konstanta
            return stof(node->value) ;
        }
        if(node->type == 3){
            int idx = stoi(node->value) ;
            return ulaz[idx] ;
        }
        if(node->type == 0){
            float x = izracunajR(node->left, ulaz) ; //0 ima samo lijevu granu
            return funkcija0(node->value, x) ;
        }
        if(node->type == 1){
            float x = izracunajR(node->left, ulaz) ;
            float y = izracunajR(node->right, ulaz) ;
            return funkcija1(node->value, x, y) ;
        }
        throw(invalid_argument("nepostojeci tip" + node->value)) ;
    }
string FunctionBinaryTree::toString(tonka::NodeT* node) {
    if (!node) return "";

    // konst
    if (node->type == 2) {
        return node->value;
    }
    //var
    if(node->type == 3){
        if(variable_names.empty()){
            return "x" + node->value ;
        }
        int num = std::stoi(node->value);
        return variable_names[num] ;
    }   
    // f0
    if (node->type == 0) {
        return node->value + "(" + toString(node->left) + ")";
    }
    // f1
    if (node->type == 1) {
        return "(" + toString(node->left) + " " + node->value + " " + toString(node->right) + ")";
    }    
    throw runtime_error("Unknown node type.");
}
string FunctionBinaryTree::toString() {
    return toString(root);
}
int FunctionBinaryTree::countNodes(tonka::NodeT* node) {
    if (node == nullptr) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}
bool FunctionBinaryTree::operator==(const FunctionBinaryTree other){
    return areNodesEqual(this->root, other.root);
}

bool FunctionBinaryTree::areNodesEqual(tonka::NodeT* node1, tonka::NodeT* node2){
    if (node1 == nullptr && node2 == nullptr) return true;
    if (node1 == nullptr || node2 == nullptr) return false;    
    if (node1->type != node2->type || node1->value != node2->value) return false;
    return areNodesEqual(node1->left, node2->left) && areNodesEqual(node1->right, node2->right);
}

tonka::NodeT* tonka::NodeT::copy()  {
    tonka::NodeT* newNode = new tonka::NodeT(value, type); // Create a new node with the current node's value and type
    if (left) {
        newNode->left = left->copy(); // Recursively copy the left child
    }
    if (right) {
        newNode->right = right->copy(); // Recursively copy the right child
    }
    return newNode; // Return the new node
}

void FunctionBinaryTree::fitness(){

    float fit = 0 ;
    for (auto& map : Parameters::maps){
        
        Bird bird;      
        Simulator simulator(map) ;
        simulator.initialize(bird) ;

        GPcontroller controller(*this) ;
    
        // Main game loop
        while (simulator.isRunning()) {
            //simulator.handleEvents(agent);
            controller.action(bird, simulator);

            simulator.update(bird);

            if (bird.distance >= MAX_MAP_SIZE){
                break;
            }
        }
        fit += bird.distance ;
    }
    this->fit = fit ;
}

FunctionBinaryTree::FunctionBinaryTree(const string& expression, int dimenzija) {
    dim = dimenzija;
    size_t pos = 0;
    root = parseFromString(expression, pos);
}

bool FunctionBinaryTree::isOperator(const string& token) {
    return find(functions1.begin(), functions1.end(), token) != functions1.end();
}

bool FunctionBinaryTree::isFunction(const string& token) {
    return find(functions0.begin(), functions0.end(), token) != functions0.end();
}

string FunctionBinaryTree::getNextToken(const string& expression, size_t& pos) {
    while (pos < expression.length() && (expression[pos] == ' ' || expression[pos] == '(' || expression[pos] == ')')) {
        pos++;
    }
    
    if (pos >= expression.length()) {
        return "";
    }
    
    string token;
    
    // Handle variables (x0, x1, etc.)
    if (expression[pos] == 'x') {
        token += expression[pos++];
        while (pos < expression.length() && isdigit(expression[pos])) {
            token += expression[pos++];
        }
        return token;
    }
    
    // Handle numbers (including decimals)
    if (isdigit(expression[pos]) || expression[pos] == '-' || expression[pos] == '.') {
        while (pos < expression.length() && 
               (isdigit(expression[pos]) || expression[pos] == '.' || expression[pos] == '-')) {
            token += expression[pos++];
        }
        return token;
    }
    
    // Handle operators and functions
    while (pos < expression.length() && 
           !isspace(expression[pos]) && 
           expression[pos] != '(' && 
           expression[pos] != ')') {
        token += expression[pos++];
    }
    
    return token;
}

tonka::NodeT* FunctionBinaryTree::createNodeFromToken(const string& token) {
    if (token.empty()) {
        return nullptr;
    }
    
    // Handle variables (x0, x1, etc.)
    if (token[0] == 'x') {
        string index = token.substr(1);
        return new tonka::NodeT(index, 3);  // type 3 for variables
    }
    
    // Handle numbers
    if (isdigit(token[0]) || token[0] == '-' || token[0] == '.') {
        return new tonka::NodeT(token, 2);  // type 2 for constants
    }
    
    // Handle functions
    if (isFunction(token)) {
        return new tonka::NodeT(token, 0);  // type 0 for unary functions
    }
    
    // Handle operators
    if (isOperator(token)) {
        return new tonka::NodeT(token, 1);  // type 1 for binary operators
    }
    
    throw runtime_error("Invalid token: " + token);
}

tonka::NodeT* FunctionBinaryTree::parseFromString(const string& expression, size_t& pos) {
    string token = getNextToken(expression, pos);
    if (token.empty()) {
        return nullptr;
    }
    
    tonka::NodeT* node = createNodeFromToken(token);
    
    if (node->type == 0) {  // Unary function
        node->left = parseFromString(expression, pos);
    }
    else if (node->type == 1) {  // Binary operator
        node->left = parseFromString(expression, pos);
        node->right = parseFromString(expression, pos);
    }
    
    return node;
}
