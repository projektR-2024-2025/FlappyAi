#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include "../../Flappy/src/Simulator.h"
#include "../../Flappy/src/Controller.h"

#define GENERATION_SIZE 10
#define GENERATION_COUNT 500
#define MUTATION_RATE 0.70

struct Individual {
    NeuralNetwork nn;
    double fitness;

    Individual(const NeuralNetwork& nn) : nn(nn), fitness(0) {}
};

double evaluateFitness(NeuralNetwork& nn, Simulator& simulator, Agent& agent) {
    NeuralController controller(nn);
    int length = 0;

    while (simulator.isRunning()) {
        if (!simulator.handleEvents(agent)) {
            controller.action(agent, simulator);
        }
        simulator.update(agent);
        ++length;
        if (length >= 300) break;
    }
    return length;
}

void mutate(NeuralNetwork& nn, double mutationRate) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    for (auto& layer_weights : nn.weights) {
        for (auto& neuron_weights : layer_weights) {
            for (auto& weight : neuron_weights) {
                if (dis(gen) < mutationRate) {
                    weight += dis(gen);
                }
            }
        }
    }

    for (auto& layer_biases : nn.biases) {
        for (auto& bias : layer_biases) {
            if (dis(gen) < mutationRate) {
                bias += dis(gen);
            }
        }
    }
}

Individual selectFromTopHalf(const std::vector<Individual>& population) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, population.size() / 2 - 1);

    double index = dis(gen);

    return population[index];
}

NeuralNetwork crossover(const NeuralNetwork& parent1, const NeuralNetwork& parent2) {
    NeuralNetwork child = parent1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (size_t i = 0; i < parent1.weights.size(); ++i) {
        for (size_t j = 0; j < parent1.weights[i].size(); ++j) {
            for (size_t k = 0; k < parent1.weights[i][j].size(); ++k) {
                child.weights[i][j][k] = dis(gen) ? parent1.weights[i][j][k] : parent2.weights[i][j][k];
            }
        }
    }

    for (size_t i = 0; i < parent1.biases.size(); ++i) {
        for (size_t j = 0; j < parent1.biases[i].size(); ++j) {
            child.biases[i][j] = dis(gen) ? parent1.biases[i][j] : parent2.biases[i][j];
        }
    }

    return child;
}

Individual tournamentSelection(const std::vector<Individual>& population, int tournamentSize) {
    std::vector<Individual> tournament;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, population.size() - 1);

    for (int i = 0; i < tournamentSize; ++i) {
        int randomIndex = dis(gen);
        tournament.push_back(population[randomIndex]);
    }

    std::sort(tournament.begin(), tournament.end(), [](const Individual& a, const Individual& b) {
        return a.fitness > b.fitness;
        });

    return tournament[0];
}

int main() {
    // definiranje strukture neuronske mreze
    std::vector<int> layers = { 4, 5, 2 }; // npr. 4 input neurona, 5 hidden i 2 output

    // inicijalizacija populacije
    std::vector<Individual> population;
    for (int i = 0; i < GENERATION_SIZE; ++i) {
        NeuralNetwork nn(layers);
        population.emplace_back(nn);
    }

    // evaluacija fitnessa
    for (auto& individual : population) {
        Simulator simulator;
        Agent agent;
        individual.fitness = evaluateFitness(individual.nn, simulator, agent);
    }

    // sortiraj populaciju po fitnessu
    std::sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness > b.fitness;
        });

    // genetski algoritam
    double mutationRate = MUTATION_RATE;
    for (int generation = 0; generation < GENERATION_COUNT; ++generation) {
        std::vector<Individual> newPopulation;

        // elitizam
        newPopulation.push_back(population[0]);
        newPopulation.push_back(population[1]);

        // crossover + mutacija
        while (newPopulation.size() < GENERATION_SIZE) {
            //Individual parent1 = selectFromTopHalf(population);
            //Individual parent2 = selectFromTopHalf(population);
            Individual parent1 = tournamentSelection(population, 10);
            Individual parent2 = tournamentSelection(population, 10);
            NeuralNetwork childNN = crossover(parent1.nn, parent2.nn);
            mutate(childNN, mutationRate);
            newPopulation.emplace_back(childNN);
        }

        // evaluacija fitnessa
        for (auto& individual : newPopulation) {
            Simulator simulator;
            Agent agent;
            individual.fitness = evaluateFitness(individual.nn, simulator, agent);
        }

        // sortiraj populaciju po fitnessu
        std::sort(newPopulation.begin(), newPopulation.end(), [](const Individual& a, const Individual& b) {
            return a.fitness > b.fitness;
            });

        population = newPopulation;

        // printaj najboljeg
        std::cout << "Generation " << generation << " - Best Fitness: " << population[0].fitness << std::endl;
    }

    // runnaj najbolju jedinku (ali na drugoj mapi)
    Simulator simulator("map2.txt");
    Agent agent;
    NeuralController controller(population[0].nn);
    while (simulator.isRunning()) {
        if (simulator.handleEvents(agent) == false)
            controller.action(agent, simulator);
        simulator.update(agent);
        simulator.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(60)); // Control game speed
    }

    return 0;
}