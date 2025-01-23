#include "CGP1.h"
#include "../Simulator.h"
#include "../Controller.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <random>
#include <fstream>
#include <string>
#include <omp.h>
#include <sstream>

using namespace std;

vector<CGP1Individual> CGP1::generatePopulation(int rows, int columns, int levelsBack, int inputs, int outputs) {
    vector<CGP1Individual> population;

    #pragma omp parallel for
    for (int i = 0; i < POPULATION; i++) {

        random_device rd;
        mt19937 gen(rd());

        uniform_int_distribution<> operandDis(1, NUM_OPERANDS);
        uniform_int_distribution<> connectionDis(0, rows * columns + inputs - 1);
        uniform_int_distribution<> outputDis(0, rows * columns + inputs - 1);

        vector<CGP1Node> genes;
        vector<CGP1Output> outputGene;

        for (size_t k = 0; k < inputs; k++) {
            CGP1Node node;
            node.used = false;
            node.connection1 = -1;
            node.connection2 = -1;
            node.operand = -1;
            genes.push_back(node);
        }

        #pragma omp parallel for
        for (int j = inputs; j < rows * columns + inputs; j++) {
            CGP1Node node;
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
            CGP1Output output;

            output.connection = outputDis(gen);

            outputGene.push_back(output);
        }

        CGP1Individual individual(genes, outputGene, rows, columns, levelsBack, inputs, outputs);
        individual.resolveLoops();
        #pragma omp critical
        population.push_back(individual);

        std::cout << "|";
    }
    std::cout << endl;

    return population;
}

// point mutacija
vector<CGP1Individual> CGP1::mutate(int numMut, CGP1Individual parent) {
    vector<CGP1Individual> population;
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
        vector<CGP1Node> genes = parent.genes;
        vector<CGP1Output> outputGene = parent.outputGene;

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

        CGP1Individual individual(genes, outputGene, parent.rows, parent.columns, parent.levelsBack, parent.inputs, parent.outputs);
        individual.resolveLoops();
        population.push_back(individual);
    }

    return population;
}

// goldman mutacija
vector<CGP1Individual> CGP1::mutate(CGP1Individual parent) {
    vector<CGP1Individual> population;
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
        vector<CGP1Node> genes = parent.genes;
        vector<CGP1Output> outputGene = parent.outputGene;
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

        CGP1Individual individual(genes, outputGene, parent.rows, parent.columns, parent.levelsBack, parent.inputs, parent.outputs);
        individual.resolveLoops();

        //#pragma omp critical
        population.push_back(individual);
    }

    return population;

}

CGP1Individual CGP1::runCGP() {
    vector<CGP1Individual> population;
    int bestInd = 0, generacija = 0;

    double time;
    time = omp_get_wtime();

    population = generatePopulation(rows, columns, levelsBack, inputs, outputs);

    std::cout << "Vrijeme: " << (omp_get_wtime() - time) << "s" << endl;

    for (generacija = 0; generacija < generations; generacija++) {
        TYPE bestFit = -1;
        bestInd = 0;
        vector<int> bestInds;
        random_device rd;
        mt19937 gen(rd());
        for (int i = 0; i < POPULATION; i++) {

            float totalDistance = 0.f;
            for (auto& map : Parameters::maps) {
                Simulator simulator(map);
                Bird bird;
                Controller* controller = new CGP1Controller(population[i]);
                simulator.initialize(bird);
                while (simulator.isRunning()) {
                    simulator.update(bird);
                    controller->action(bird, simulator);
                    if (bird.distance >= MAX_MAP_SIZE)
                        break;
                }
                totalDistance += population[i].calculateFitness((TYPE) bird.distance);
            }

            TYPE fit = totalDistance / Parameters::maps.size();
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

        std::cout << "Gen: " << generacija << "; Fitness: " << bestFit << "; Indeks: " << bestInd << endl;

        if (bestFit >= MAX_MAP_SIZE)
            break;

        if (generacija != generations - 1)
            population = mutate(population[bestInd]);
    }

    actualGens = generacija;

    try {
        ofstream outFile(bestFile);
        if (outFile.is_open()) {
            outFile << population[bestInd];
            outFile.close();
            std::cout << "CGP written to text file." << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error writing CGP: " << e.what() << endl;
    }

    return population[bestInd];
}

CGP1Controller* CGP1::CGPMain(ActionType action) {
    CGP1 cgp(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS);
    CGP1Individual ind;

    if (action == BEST) {
        try {
            std::ifstream inFile(bestFile);
            if (inFile.is_open()) {
                ind = CGP1Individual::deserialize(inFile);
                inFile.close();
                std::cout << "CGP read from text file." << std::endl;
                ind.printFuction();
            }
        }
        catch (const exception& e) {
            cerr << "Error loading CGP: " << e.what() << endl;
        }
    }
    else if (action == TRAIN) {
        ind = cgp.runCGP();
        ind.printFuction();
    }

    return new CGP1Controller(ind);
}