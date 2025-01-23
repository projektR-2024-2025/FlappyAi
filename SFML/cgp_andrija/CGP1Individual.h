#ifndef CGP1INDIVIDUAL_H
#define CGP1INDIVIDUAL_H
#define TYPE double

#include <vector>
#include <sstream>
#include "CGP1Node.h"
#include "CGP1Output.h"

// klasa koja opsiuje pojedinca
class CGP1Individual {
private:
    std::string evalFunction(int nodeNum);
    void isUsed(int nodeNum);
    bool loopFinder(int nodeNum, std::vector<int> nodeSet);
    TYPE evalNode(int nodeNum);
    void clearInd();
public:
    std::vector<CGP1Node> genes;
    std::vector<CGP1Output> outputGene;
    std::vector<std::vector<int>> branches;
    int rows;
    int columns;
    int levelsBack;
    int inputs;
    int outputs;
    int evalDone;

    CGP1Individual();
    CGP1Individual(std::vector<CGP1Node> genes, std::vector<CGP1Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs);
    CGP1Individual(std::vector<CGP1Node> genes, std::vector<CGP1Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs, bool evalDone);

    void printNodes();
    void printFuction();
    void evaluateValue(std::vector<TYPE> input);
    void evaluateUsed();
    TYPE calculateFitness(TYPE lenght);
    static CGP1Individual deserialize(std::istream& is);
    bool findLoops(int nodeNum, std::vector<int> nodeSet);
    void resolveLoops();

    friend std::ostream& operator<<(std::ostream& os, const CGP1Individual& ind) {
        os << ind.rows << " " << ind.columns << " " << ind.levelsBack << " "
            << ind.inputs << " " << ind.outputs << " " << ind.evalDone << "\n";

        os << ind.genes.size() << "\n";
        for (const auto& gene : ind.genes)
            os << gene << "\n";

        os << ind.outputGene.size() << "\n";
        for (const auto& output : ind.outputGene)
            os << output << "\n";

        return os;
    }

    // Deserialize the Individual object
    friend std::istream& operator>>(std::istream& is, CGP1Individual& ind) {
        is >> ind.rows >> ind.columns >> ind.levelsBack
            >> ind.inputs >> ind.outputs >> ind.evalDone;

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