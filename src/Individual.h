#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#define TYPE double

#include <vector>
#include "Node.h"
#include "Output.h"

// klasa koja opsiuje pojedinca
class Individual {
private:
    void isUsed(int nodeNum);
    bool loopFinder(int nodeNum, std::vector<int> nodeSet);
    TYPE evalNode(int nodeNum);
    void clearInd();

public:
    std::vector<Node> genes;
    std::vector<Output> outputGene;
    std::vector<std::vector<int>> branches;
    int rows;
    int columns;
    int levelsBack;
    int inputs;
    int outputs;
    TYPE fitness;
    int evalDone;

    Individual();
    Individual(std::vector<Node> genes, std::vector<Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs);
    Individual(std::vector<Node> genes, std::vector<Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs, bool evalDone, int fitness);

    void printNodes();
    void evaluateValue(std::vector<TYPE> input);
    void evaluateUsed();
    TYPE calculateFitness(TYPE lenght);
    static Individual deserialize(std::istream& is);
    bool findLoops(int nodeNum, std::vector<int> nodeSet);
    void resolveLoops();

    friend std::ostream& operator<<(std::ostream& os, const Individual& ind) {
        os << ind.rows << " " << ind.columns << " " << ind.levelsBack << " "
            << ind.inputs << " " << ind.outputs << " " << ind.fitness << " "
            << ind.evalDone << "\n";

        os << ind.genes.size() << "\n";
        for (const auto& gene : ind.genes)
            os << gene << "\n";

        os << ind.outputGene.size() << "\n";
        for (const auto& output : ind.outputGene)
            os << output << "\n";

        return os;
    }

    // Deserialize the Individual object
    friend std::istream& operator>>(std::istream& is, Individual& ind) {
        is >> ind.rows >> ind.columns >> ind.levelsBack
            >> ind.inputs >> ind.outputs >> ind.fitness >> ind.evalDone;

        size_t genesSize, outputGeneSize;
        is >> genesSize;
        ind.genes.resize(genesSize);
        for (auto& gene : ind.genes)
            is >> gene;

        is >> outputGeneSize;
        ind.outputGene.resize(outputGeneSize);
        for (auto& output : ind.outputGene)
            is >> output;

        return is;
    }
};

#endif
