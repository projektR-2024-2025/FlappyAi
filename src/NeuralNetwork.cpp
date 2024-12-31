#include "NeuralNetwork.h"
#include <cmath>
#include <iostream>
#include <random>

// konstruktor koji inicijalizira neuronsku mrezu
NeuralNetwork::NeuralNetwork(const std::vector<int>& layers) : layers(layers) {
    // koristi random brojeve za inicijalizaciju
    // pomocu normalne razdiobe
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // inicijaliziraj weights i biases
    // pocinjemo od 1 jer moramo za ovaj korak preskociti input layer
    // posto gledamo prethodni layer
    for (size_t l = 1; l < layers.size(); ++l) {
        // broj neurona u trenutnom layeru
        int num_neurons_in_current_layer = layers[l];
        // broj neurona u prethodnom layeru
        int num_neurons_in_prev_layer = layers[l - 1];

        // inicijalizacija weightova u random values
        // layer_weights[0] predstavlja weightove izmedju 0. neurona trenutnog layera
        // s neuronima iz prethodnog layera
        std::vector<std::vector<double>> layer_weights(num_neurons_in_current_layer, std::vector<double>(num_neurons_in_prev_layer));
        for (auto& neuron_weights : layer_weights) {
            for (auto& weight : neuron_weights) {
                weight = dis(gen);
            }
        }
        weights.push_back(layer_weights);

        // inicijaliziranje biasa u random values (svaki neuron jedan bias)
        std::vector<double> layer_biases(num_neurons_in_current_layer);
        for (auto& bias : layer_biases) {
            bias = dis(gen);
        }
        biases.push_back(layer_biases);
    }
}

// metoda za predvidjanje rezultata; uzima input vrijednosti i provede ih kroz mrezu
std::vector<double> NeuralNetwork::predict(const std::vector<double>& input) {
    // pocetne aktivacije su jednake inputu
    std::vector<double> activations = input;

    // iteriraj se kroz layere (tretirajuci hidden layer kao layer 0)
    for (size_t l = 0; l < weights.size(); ++l) {
        // kreiraj novi vector velicine jednake broju neurona u sljedecem layeru
        std::vector<double> new_activations(layers[l + 1]);

        for (size_t j = 0; j < new_activations.size(); ++j) {
            // izracunaj bias za neuron j u layeru l (ako je l == 0
            // onda racunamo weightove izmedju input layera i hidden layera,
            // znaci ovdje bi to bio za neuron j u hidden layeru)
            double z = biases[l][j];

            // za isti taj neuron izracunaj pobudu
            for (size_t k = 0; k < activations.size(); ++k) {
                z += weights[l][j][k] * activations[k];
            }

            // sigmoidna funkcija pobude
            new_activations[j] = 1.0 / (1.0 + std::exp(-z));
        }

        activations = new_activations;
    }

    return activations;
}