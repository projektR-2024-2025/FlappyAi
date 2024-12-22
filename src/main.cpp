#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "Simulator.h"
#include "Agent.h"
#include "Controller.h"
#include "TreeControll.h"

int populationSize = 500;
int crossingNumber = 10000;
int populationCullings = 50; // sort take half of the best;

void printPopulation(Tree population[]) {
    for (int i = 0; i < populationSize; i++) {
        std::cout << i << ":    ";
        population[i].printTree();
        std::cout << "                                              " << population[i].fitness << "\n";
    }
}

Node* createOperatorNode(int a = -1, int b = -1, int c = -1){
    if (a == -1) a = rand() % 12;
    if (b == -1) b = 12 + rand() % 7;
    if (c == -1) c = 12 + rand() % 7;

    Node* returnNode = new Node(a);
    if (a < 11) returnNode->left = new Node(b);
    else returnNode->left = nullptr;
    returnNode->right = new Node(c);

    return returnNode;
}

double calculateFitness(Tree& calcTree) {
    // Racunanje fitnessa
    Simulator simulator;
    Agent agent;
    GPContreoller* controller = new GPContreoller;  // plug in your controller here
    controller->passedRoot = calcTree.root;
    controller->distFlown = 0;

    int guard = simulator.mapLen + 10;
    // Main game loop
    while (simulator.isRunning() && controller->distFlown < guard) {
        //simulator.render();
        if (simulator.handleEvents(agent) == false)
            controller->action(agent, simulator);
        simulator.update(agent);
        controller->distFlown += 1; // counting for fitness
        //simulator.render();
    }
    //simulator.render();
    double fitness = controller->distFlown / simulator.mapLen;
    delete controller;
    return fitness;
}
double calculateFitnessDisplay(Tree& calcTree) {
    // Racunanje fitnessa
    Simulator simulator;
    Agent agent;
    GPContreoller* controller = new GPContreoller;  // plug in your controller here
    controller->passedRoot = calcTree.root;
    controller->distFlown = 0;

    int guard = simulator.mapLen + 10;
    // Main game loop
    while (simulator.isRunning() && controller->distFlown < guard) {
        //calcTree.printTree();
        simulator.render();
        if (simulator.handleEvents(agent) == false)
            controller->action(agent, simulator);
        simulator.update(agent);
        controller->distFlown += 1; // counting for fitness
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Control game speed
    }
    //calcTree.printTree();
    simulator.render();
    double fitness = controller->distFlown / simulator.mapLen;
    delete controller;
    return fitness;
}

void mutate(Node*& n) {
    if (n == nullptr) return;

    //change operator
    if (!(n->left == nullptr && n->right == nullptr)) {
        mutate(n->left);
        mutate(n->right);
    }
    // Change the leaf to a new operator.
    else if (rand() % 100 < 10) {
        delete n;
        n = createOperatorNode();
    }
}


void deleteCrossMutate(Tree population[], int delInd, int par1Ind, int par2Ind) {
    //BRISI
    delete population[delInd].root;
    population[delInd].root = createOperatorNode(population[par1Ind].root->value);
    delete population[delInd].root->left;
    delete population[delInd].root->right;
    //KRIZAJ
    Node* tmp;
    tmp = population[par1Ind].root->left->deepNodeCopy();
    population[delInd].root->left = tmp;   //KRIVO  treba napraviti nove instance i iskopirati ih u pop[delInd]


    tmp = population[par2Ind].root->right->deepNodeCopy();
    population[delInd].root->right = tmp; //KRIVO

    //MUTATE
    mutate(population[delInd].root);

    // Calculate new fitness.
    population[delInd].printTree();
    population[delInd].fitness = calculateFitness(population[delInd]);
}

void simulatePopulation() {
    Tree population[populationSize];

    // Inicijalizacija populacije
    for (int i = 0; i < populationSize; i++) {
        Node* tmp = createOperatorNode();
        population[i] = Tree();
        population[i].root = tmp;
        population[i].fitness = calculateFitness(population[i]);
    }

    // Ispis populacije i fitnessa.
    printPopulation(population);

        // Simulacija crossingNumber puta;
    for (int i = 0; i < crossingNumber; i++) {
        std::cout << i << " ";
        // Odabir.
        int j1, j2, j3;
        j1 = rand() % populationSize;
        j2 = j1;
        while (j1 == j2) j2 = rand() % populationSize;
        j3 = j2;
        while (j3 == j2 || j3 == j1) j3 = rand() % populationSize;

        // Brisanje i krizanje.
        if (population[j1].fitness <= population[j2].fitness && population[j1].fitness <= population[j3].fitness)
            deleteCrossMutate(population, j1, j2, j3);
        else if (population[j2].fitness <= population[j1].fitness && population[j2].fitness <= population[j3].fitness)
            deleteCrossMutate(population, j2, j1, j3);
        else if (population[j3].fitness <= population[j1].fitness && population[j3].fitness <= population[j2].fitness)
            deleteCrossMutate(population, j3, j1, j2);
        std::cout << "\n";
        // Mutacija.
    }
    std::cout << "\n";

    printPopulation(population);

        // Sort
        // Cull
    // Ponovi za populationCullings puta;
    // sortiraj jos jednom
    std::cout << "Indeks jedinke za prikaz: ";
    int p;
    std::cin >> p;
    std::cout << calculateFitnessDisplay(population[p]) << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
}


int main() {
    srand((time(0)));

    std::cout << "Load/Simulate? (0/1)";
    char option;
    std::cin >> option;
    if (option == '0') int sjlajdks; // OVDJE CE IC LOAD POZIV
    else if (option == '1') simulatePopulation();

    return 0;
}
