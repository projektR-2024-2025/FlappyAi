#include "CGP.h"
#include "Simulator.h"
#include "Controller.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <random>
#include <fstream>
#include <string>
#include <omp.h>

using namespace std;

CGP::CGP() {
    this->map.map = "map1.txt";
    this->map.mapNum = 1;
}

CGP::CGP(std::string map, int mapNum) {
    this->map.map = map;
    this->map.mapNum = mapNum;
}

void CGP::switchMap() {
    switch (map.mapNum) {
    case 1:
        map.mapNum = 2;
        map.map = "map2.txt";
        break;
    case 2:
        map.mapNum = 1;
        map.map = "map1.txt";
        break;
    default:
        break;
    }
}

vector<Individual> CGP::generatePopulation(int rows, int columns, int levelsBack, int inputs, int outputs) {
    vector<Individual> population;

    #pragma omp parallel for
    for (int i = 0; i < POPULATION; i++) {

        random_device rd;
        mt19937 gen(rd());

        uniform_int_distribution<> operandDis(1, NUM_OPERANDS);
        uniform_int_distribution<> connectionDis(0, rows * columns + inputs - 1);
        uniform_int_distribution<> outputDis(0, rows * columns + inputs - 1);

        vector<Node> genes;
        vector<Output> outputGene;

        for (size_t k = 0; k < inputs; k++) {
            Node node;
            node.used = false;
            node.connection1 = -1;
            node.connection2 = -1;
            node.operand = -1;
            genes.push_back(node);
        }

        #pragma omp parallel for
        for (int j = inputs; j < rows * columns + inputs; j++) {
            Node node;
            node.used = false;
            node.operand = operandDis(gen);
            node.connection1 = connectionDis(gen);
            node.outValue = NAN;

            while (true) {
                if (node.connection1 < inputs)
                    break;
                if ((node.connection1 % columns) == (j % columns))
                    node.connection1 = connectionDis(gen);
                else if (((node.connection1 - inputs) % columns) > (((j - inputs) % columns) + levelsBack))
                    node.connection1 = connectionDis(gen);
                else if(genes.size() > node.connection1 && (genes[node.connection1].connection1 == j || genes[node.connection1].connection2 == j))
                    node.connection1 = connectionDis(gen);
                else
                    break;
            }

            node.connection2 = (node.operand >= 5) ? -1 : connectionDis(gen);

            while (true) {
                if (node.connection2 < inputs)
                    break;
                if ((node.connection2 % columns) == (j % columns))
                    node.connection2 = connectionDis(gen);
                else if (((node.connection2 - inputs) % columns) > (((j - inputs) % columns) + levelsBack))
                    node.connection2 = connectionDis(gen);
                else if (genes.size() > node.connection2 && (genes[node.connection2].connection1 == j || genes[node.connection2].connection2 == j))
                    node.connection2 = connectionDis(gen);
                else
                    break;
            }

            #pragma omp critical
            genes.push_back(node);
        }

        for (size_t k = 0; k < outputs; k++) {
            Output output;

            output.connection = outputDis(gen);

            outputGene.push_back(output);
        }

        Individual individual(genes, outputGene, rows, columns, levelsBack, inputs, outputs);
        individual.resolveLoops();
        #pragma omp critical
        population.push_back(individual);

        cout << "|";
    }
    cout << endl;

    return population;
}

// point mutacija
vector<Individual> CGP::mutate(int numMut, Individual parent) {
    vector<Individual> population;
    if (!parent.evalDone)
        parent.evaluateUsed();
    population.push_back(parent);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> nodDis(parent.inputs, parent.genes.size());
    uniform_int_distribution<> geneDis(0, 2);
    uniform_int_distribution<> connectionDis(0, parent.genes.size() - 1);
    uniform_int_distribution<> operandDis(1, NUM_OPERANDS);
    uniform_int_distribution<> outputDis(0, parent.outputs - 1);

    for (int n = 0; n < POPULATION - 1; n++) {
        vector<Node> genes = parent.genes;
        vector<Output> outputGene = parent.outputGene;

        for (int z = parent.inputs; z < genes.size(); z++)
            genes[z].used = false;

        for (int i = 0; i < numMut; i++) {
            int mut = geneDis(gen);
            int cell = nodDis(gen);
            if (cell == parent.genes.size()) {
                outputGene[outputDis(gen)].connection = connectionDis(gen);
                continue;
            }
            if (mut == 0)
                genes[cell].operand = operandDis(gen);
            else if (mut == 1)
                genes[cell].connection1 = connectionDis(gen);
            else if (mut == 2)
                genes[cell].connection2 = connectionDis(gen);

            genes[cell].connection2 = (genes[cell].operand >= 5) ? -1 : connectionDis(gen);

            while (true) {
                if (genes[cell].connection1 < parent.inputs)
                    break;
                if ((genes[cell].connection1 % parent.columns) == (cell % parent.columns))
                    genes[cell].connection1 = connectionDis(gen);
                else if (((genes[cell].connection1 - parent.inputs) % parent.columns) > (((cell - parent.inputs) % parent.columns) + parent.levelsBack))
                    genes[cell].connection1 = connectionDis(gen);
                else
                    break;
            }

            while (true) {
                if (genes[cell].connection2 < parent.inputs)
                    break;
                if ((genes[cell].connection2 % parent.columns) == (cell % parent.columns))
                    genes[cell].connection2 = connectionDis(gen);
                else if (((genes[cell].connection2 - parent.inputs) % parent.columns) > (((cell - parent.inputs) % parent.columns) + parent.levelsBack))
                    genes[cell].connection2 = connectionDis(gen);
                else
                    break;
            }
        }

        Individual individual(genes, outputGene, parent.rows, parent.columns, parent.levelsBack, parent.inputs, parent.outputs);
        individual.resolveLoops();
        population.push_back(individual);
    }

    return population;
}

// goldman mutacija
vector<Individual> CGP::mutate(Individual parent) {
    vector<Individual> population;
    if (!parent.evalDone)
        parent.evaluateUsed();
    population.push_back(parent);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> nodDis(parent.inputs, parent.genes.size());
    uniform_int_distribution<> geneDis(0, 2);
    uniform_int_distribution<> connectionDis(0, parent.genes.size() - 1);
    uniform_int_distribution<> operandDis(1, NUM_OPERANDS);
    uniform_int_distribution<> outputDis(0, parent.outputs - 1);

    // upali ovo ako zelis cpu stress test
    //#pragma omp parallel for
    for (int n = 0; n < POPULATION - 1; n++) {
        vector<Node> genes = parent.genes;
        vector<Output> outputGene = parent.outputGene;
        bool isActive = false;

        while (!isActive) {
            int mut = geneDis(gen);
            int cell = nodDis(gen);
            if (cell == parent.genes.size()) {
                outputGene[outputDis(gen)].connection = connectionDis(gen);
                break;
            }
            if (mut == 0) {
                genes[cell].operand = operandDis(gen);
                
                if (genes[cell].operand >= 5 && genes[cell].connection2 != -1)
                    genes[cell].connection2 = -1;
                else if (genes[cell].operand < 5 && genes[cell].connection2 == -1)
                    genes[cell].connection2 = connectionDis(gen);
            }
            else if (mut == 1)
                genes[cell].connection1 = connectionDis(gen);
            else if (mut == 2 && genes[cell].operand >= 5)
                continue;
            else if (mut == 2)
                genes[cell].connection2 = connectionDis(gen);

            while (true) {
                if (genes[cell].connection1 < parent.inputs)
                    break;
                if ((genes[cell].connection1 % parent.columns) == (cell % parent.columns))
                    genes[cell].connection1 = connectionDis(gen);
                else if (((genes[cell].connection1 - parent.inputs) % parent.columns) > (((cell - parent.inputs) % parent.columns) + parent.levelsBack))
                    genes[cell].connection1 = connectionDis(gen);
                else
                    break;
            }

            while (true) {
                if (genes[cell].connection2 < parent.inputs)
                    break;
                if ((genes[cell].connection2 % parent.columns) == (cell % parent.columns))
                    genes[cell].connection2 = connectionDis(gen);
                else if (((genes[cell].connection2 - parent.inputs) % parent.columns) > (((cell - parent.inputs) % parent.columns) + parent.levelsBack))
                    genes[cell].connection2 = connectionDis(gen);
                else
                    break;
            }

            isActive = genes[cell].used;
        }

        for (int z = parent.inputs; z < genes.size(); z++)
            genes[z].used = false;

        Individual individual(genes, outputGene, parent.rows, parent.columns, parent.levelsBack, parent.inputs, parent.outputs);
        individual.resolveLoops();

        //#pragma omp critical
        population.push_back(individual);
    }

    return population;

}

Individual CGP::runCGP(int generations, int rows, int columns, int levelsBack, int inputs, int outputs, int mutations) {
    vector<Individual> population;
    int bestInd = 0, generacija = 0;

    double time;
    time = omp_get_wtime();

    population = generatePopulation(rows, columns, levelsBack, inputs, outputs);

    cout << "Vrijeme: " << (omp_get_wtime() - time) << endl;

    for (generacija = 0; generacija < generations; generacija++) {
        TYPE bestFit = 0;
        bestInd = 0;
        vector<int> bestInds;
        random_device rd;
        mt19937 gen(rd());
        for (int i = 0; i < POPULATION; i++) {

            int lenght = 0;
            Simulator simulator(map.map);
            Agent agent;
            CGPController* controller = new CGPController;

            while (simulator.isRunning()) {
                controller->action(agent, simulator, population[i]);
                simulator.update(agent);
                lenght++;
                if (lenght >= MAX_MAP_SIZE)
                    break;
            }

            TYPE fit = population[i].calculateFitness(lenght);
            if (fit > bestFit) {
                bestFit = fit;
                bestInds.clear();
                bestInds.push_back(i);
            }
            else if (fit == bestFit)
                bestInds.push_back(i);
        }

        if (bestInds.size() > 1)
            bestInds.erase(bestInds.begin());

        uniform_int_distribution<> bestDis(0, bestInds.size() - 1);

        bestInd = bestInds[bestDis(gen)];

        cout << "Gen: " << generacija << "; Fitness: " << bestFit << "; Indeks: " << bestInd << "; Mapa: " << map.map << endl;

        if ((generacija != 0 && generacija % MAP_CHANGE == 0))
            switchMap();

        //if (bestFit == MAX_MAP_SIZE)
        //    break;

        if (generacija != generations - 1)
            population = mutate(population[bestInd]);
    }

    actualGens = generacija;

    population[bestInd].printNodes();
    std::ofstream outFile("CGP_best.txt");
    if (outFile.is_open()) {
        outFile << population[bestInd];
        outFile.close();
        std::cout << "Object written to text file." << std::endl;
    }

    return population[bestInd];
}