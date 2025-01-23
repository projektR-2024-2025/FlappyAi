#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>

class NeuralNetwork {
public:
    NeuralNetwork(const std::vector<int>& layers);
    std::vector<double> predict(const std::vector<double>& input);

    std::vector<int> layers;
    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<std::vector<double>> biases;
};

#endif