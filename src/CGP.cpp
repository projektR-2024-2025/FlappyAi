#include "CGP.h"
#include "Simulator.h"
#include "Controller.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <string>

using namespace std;

CGP::CGP() {
    this->map = "map1.txt";
}

CGP::CGP(std::string map) {
    this->map = map;
}

vector<Individual> CGP::generatePopulation(int rows, int columns, int levelsBack, int inputs, int outputs) {
    vector<Individual> population;

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> operandDis(1, NUM_OPERANDS);
    uniform_int_distribution<> connectionDis(0, rows * columns + inputs - 1);
    uniform_int_distribution<> outputDis(0, rows * columns + inputs - 1);

    for (size_t i = 0; i < 5; i++) {

        vector<Node> genes;
        vector<Output> outputGene;

        for (size_t i = 0; i < inputs; i++) {
            Node node;
            node.used = true;
            genes.push_back(node);
        }

        for (size_t j = inputs; j < rows * columns + inputs; j++) {
            Node node;
            node.used = false;
            node.operand = operandDis(gen);
            node.connection1 = connectionDis(gen);

            while (true) {
                if (node.connection1 < inputs)
                    break;
                if ((node.connection1 % columns) == (j % columns))
                    node.connection1 = connectionDis(gen);
                else if (((node.connection1 - inputs) % columns) > (((j - inputs) % columns) + levelsBack))
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
                else
                    break;
            }

            genes.push_back(node);
        }

        for (size_t k = 0; k < outputs; k++) {
            Output output;

            output.connection = outputDis(gen);

            outputGene.push_back(output);
        }

        Individual individual(genes, outputGene, rows, columns, levelsBack, inputs, outputs);
        population.push_back(individual);
    }

    return population;
}

vector<Individual> CGP::mutate(int numMut, Individual parent) {
    vector<Individual> population;
    population.push_back(parent);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> nodDis(parent.inputs, parent.genes.size() - 1);
    uniform_int_distribution<> geneDis(0, 2);
    uniform_int_distribution<> connectionDis(0, parent.genes.size() - 1);
    uniform_int_distribution<> operandDis(1, NUM_OPERANDS);
    uniform_int_distribution<> outputDis(0, parent.outputs - 1);

    for (int n = 0; n < 4; n++) {
        vector<Node> genes = parent.genes;
        vector<Output> outputGene = parent.outputGene;

        for (int z = parent.inputs; z < genes.size(); z++)
            genes[z].used = false;

        for (int i = 0; i < numMut; i++) {
            int mut = geneDis(gen);
            int cell = nodDis(gen);
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

        outputGene[outputDis(gen)].connection = connectionDis(gen);

        Individual individual(genes, outputGene, parent.rows, parent.columns, parent.levelsBack, parent.inputs, parent.outputs);
        population.push_back(individual);
    }

    return population;
}

Individual CGP::runCGP(int generations, int rows, int columns, int levelsBack, int inputs, int outputs, int mutations) {
    vector<Individual> population;
    int minInd = 0;
    population = generatePopulation(rows, columns, levelsBack, inputs, outputs);

    for (int p = 0; p < generations; p++) {
        TYPE min = -1;
        minInd = 0;
        for (int i = 0; i < 5; i++) {

            int lenght = 0;
            Simulator simulator(map);
            Agent agent;
            MyController* controller = new MyController;

            while (simulator.isRunning()) {
                controller->action(agent, simulator, population[i]);
                simulator.update(agent);
                lenght++;
                if (lenght >= MAX_MAP_SIZE)
                    break;
            }

            TYPE fit = population[i].calculateFitness(lenght);
            if (min == -1) {
                min = fit;
                minInd = 0;
            }
            else if (fit <= min) {
                min = fit;
                minInd = i;
            }
        }
        std::cout << min << endl;

        population = mutate(mutations, population[minInd]);
    }

    population[minInd].printNodes();
    std::ofstream outFile("CGP_best.txt");
    if (outFile.is_open()) {
        outFile << population[minInd];
        outFile.close();
        std::cout << "Object written to text file." << std::endl;
    }

    return population[minInd];
}