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
//vector<string> functions2 = { "<", ">", "==", "&&", "||"} ;//Type 2 functions -> subset of type 1 functions

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


tonka::Node::Node(string val, int typ) : 
  value(val), type(typ), left(nullptr), right(nullptr) 
{}



FunctionBinaryTree::FunctionBinaryTree() : root(nullptr) {}

FunctionBinaryTree::FunctionBinaryTree(int max_dubina, int dimenzija){
    //srand(time(0)); // Initialize random seed
    dim = dimenzija ;
    root = createRandomNode(max_dubina, 0);
}
    
tonka::Node* FunctionBinaryTree::createRandomNode(int max_dubina, int dubina) {
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
    tonka::Node* node = new tonka::Node(value, type);

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

void FunctionBinaryTree::printTree(tonka::Node* node) {
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
float FunctionBinaryTree::izracunajR(tonka::Node *node, vector<float> ulaz){
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
string FunctionBinaryTree::toString(tonka::Node* node) {
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
int FunctionBinaryTree::countNodes(tonka::Node* node) { 
    if (node == nullptr) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}
bool FunctionBinaryTree::operator==(const FunctionBinaryTree other){
    return areNodesEqual(this->root, other.root);
}

bool FunctionBinaryTree::areNodesEqual(tonka::Node* node1, tonka::Node* node2){
    if (node1 == nullptr && node2 == nullptr) return true;
    if (node1 == nullptr || node2 == nullptr) return false;    
    if (node1->type != node2->type || node1->value != node2->value) return false;
    return areNodesEqual(node1->left, node2->left) && areNodesEqual(node1->right, node2->right);
}

tonka::Node* tonka::Node::copy()  {
    tonka::Node* newNode = new tonka::Node(value, type); // Create a new node with the current node's value and type
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
    this->fit = fit /  Parameters::maps.size();
}

