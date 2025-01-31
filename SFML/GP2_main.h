#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>
#include "Simulator.h"
#include "Agent.h"
#include "Controller.h"
#include "Parameters.h"
#include "TreeControll.h"

const int populationSize = 500;
const int crossingNumber = 30000;
const int populationCullings = 50; // sort take half of the best;
const int displayMapNum = 0;


int printPopulation(Tree population[]) {
    double best = -0.1;
    int bestIndex = -1;
    // Ispisi sve jedinke.
    for (int i = 0; i < populationSize; i++) {
        std::cout << i << ":    ";
        population[i].printTree();
        std::cout << "                                              " << population[i].fitness << "\n";

        // Trazi najbolju jedinku.
        if (population[i].fitness > best) {
            best = population[i].fitness;
            bestIndex = i;
        }
    }

    // Ispisi najbolju jedinku:
    std::cout << "\nNAJBOLJA JEDINKA: " << bestIndex << "   (fitness = " << best << ")\n";
    std::cout << "    oblik: ";
    population[bestIndex].printTree();
    return bestIndex;
}

Node2* createOperatorNode(int a = -1, int b = -1, int c = -1) {
    if (a == -1) a = rand() % 12;
    if (b == -1) b = 12 + rand() % 8;
    if (c == -1) c = 12 + rand() % 8;

    Node2* returnNode = new Node2(a);
    if (a < 11) returnNode->left = new Node2(b);
    else returnNode->left = nullptr;
    returnNode->right = new Node2(c);

    return returnNode;
}

double calculateFitness(Tree& calcTree) {
    Node2* tmp = calcTree.root->deepNodeCopy();
    float totalDistance = 0.f;
    //for (auto& map : Parameters::maps) {
        Simulator simulator("Map1.txt");
        Bird bird;
        Controller* controller = new GP2Controller(tmp);
        simulator.initialize(bird);
        while (simulator.isRunning()) {
            simulator.update(bird);
            controller->action(bird, simulator);
            if (bird.distance >= 5000) break;
        }
        totalDistance += bird.distance;
        //delete controller;
    //}
    //totalDistance /= Parameters::maps.size();
    double fitness = totalDistance;
    return fitness;
}


void mutate(Node2*& n) {
    if (n == nullptr) return;

    // Call recursively.
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
    Node2* tmp;
    tmp = population[par1Ind].root->left->deepNodeCopy();
    population[delInd].root->left = tmp;


    tmp = population[par2Ind].root->right->deepNodeCopy();
    population[delInd].root->right = tmp;

    //MUTATE
    mutate(population[delInd].root);

    // Calculate new fitness.
    population[delInd].printTree();
    population[delInd].fitness = calculateFitness(population[delInd]);
}

void saveToFile(std::string line) {
    // Open the file in append mode
    std::ofstream outFile("best_gp2.txt", std::ios::app);

    // Check if the file was successfully opened
    if (!outFile) {
        std::cerr << "Error: Could not open file 'best_gp2.txt'." << std::endl;
        return;
    }

    // Write the string to the file followed by a newline
    outFile << line << std::endl;

    // Close the file
    outFile.close();
    std::cout << "Spremanje uspjesno!\n";
}

GP2Controller* simulatePopulation() {
    Tree population[populationSize];

    // Inicijalizacija populacije
    for (int i = 0; i < populationSize; i++) {
        Node2* tmp = createOperatorNode();
        population[i] = Tree(tmp);
        //population[i].root = tmp;
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
        // Mutacija. (nepotrebno?)
    }
    std::cout << "\n";


    // Sort
    // Cull
    // Ponovi za populationCullings puta;
    // sortiraj jos jednom

    int bestIndex = printPopulation(population);
    /*char option = '0';
    while (option != '5') {
        // izbornik
        std::cout << "\n\nOpcije:\n";
        std::cout << "  1 - ispisi populaciju\n";
        std::cout << "  2 - prikazi jedinku\n";
        std::cout << "  3 - cls\n";
        std::cout << "  4 - spremi jedinku i prekini\n";
        std::cout << "  5 - prekini program\n\n";
        std::cin >> option;

        if (option == '1') {
            printPopulation(population);
        }
        else if (option == '2') {
            int p;
            std::cout << "Indeks jedinke za prikaz: ";
            std::cin >> p;
            std::cout << "  Fitness prikazane jedinke(" << p << "): " << calculateFitness(population[p], displayMapNum) << "\n";
            std::cout << "  Oblik: ";
            population[p].printTree();
        }
        else if (option == '3') system("cls");
        else if (option == '4') {
            int p;
            std::cout << "Indeks jedinke za spremanje: ";
            std::cin >> p;
            std::cout << "spremam... " << population[p].treePrefix() << "\n";
            saveToFile(population[p].treePrefix());
            option = '5';
        }

    }*/



    std::cout << "\n\n      >>> Kraj simulacije <<<\n\n\n";
    return new GP2Controller(population[bestIndex].root);
}

/*void loadSaved() {
        std::string filename = "best_gp2.txt"; // Replace with your file name
        std::ifstream file(filename); // Open the file
        std::vector<std::string> lines; // Vector to store lines

        if (!file.is_open()) {
            std::cerr << "Error: Could not open the file 'saved.txt' " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) { // Read each line
            lines.push_back(line); // Add the line to the vector
        }

        file.close(); // Close the file

        // Output the lines to verify
        Tree population[lines.size()];
        std::cout << "Contents of the file:" << std::endl;
        for (int i = 0; i < lines.size(); i++) {

            std::cout << i << ":\n" << "    " << lines[i] << std::endl << "    ";
            population[i].parseTreePrefix(lines[i]);
            population[i].fitness = (calculateFitness(population[i], 1) + calculateFitness(population[i], 2)) / 2;
            population[i].printTree();
            std::cout << "    fitness = " << population[i].fitness << "\n\n";
        }

        int p;
        std::cout << "Indeks jedinke za prikaz: ";
        std::cin >> p;
        if (p >= lines.size()) {
            std::cout << "Prevelik index!\n";
            return;
        }
        std::cout << "  Fitness prikazane jedinke(" << p << "): " << calculateFitnessDisplay(population[p], displayMapNum) << "\n";
        std::cout << "  Oblik: ";
        population[p].printTree();
}*/
GP2Controller* GP_FC(sf::RenderWindow& window) {

    srand((time(0)));
    std::cout << "Load/Simulate? (0/1)";
    char option;
    std::cin >> option;
    if (option == '0') int abc = 1;//loadSaved();
    else if (option == '1') return simulatePopulation();
    
}