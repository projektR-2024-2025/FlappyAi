#include "CGP.h"
#include "Simulator.h"
#include "Controller.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <random>
#include <fstream>
#include <string>
#include <omp.h>

#ifdef _WIN32
#define ARIAL_FONT_PATH "C:\\Windows\\Fonts\\arial.ttf"
#else
#define ARIAL_FONT_PATH "arial.ttf"
#endif

using namespace std;

vector<CGPIndividual> CGP::generatePopulation(int rows, int columns, int levelsBack, int inputs, int outputs) {
    vector<CGPIndividual> population;

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

        CGPIndividual individual(genes, outputGene, rows, columns, levelsBack, inputs, outputs);
        individual.resolveLoops();
        #pragma omp critical
        population.push_back(individual);

        cout << "|";
    }
    cout << endl;

    return population;
}

// point mutacija
vector<CGPIndividual> CGP::mutate(int numMut, CGPIndividual parent) {
    vector<CGPIndividual> population;
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

        CGPIndividual individual(genes, outputGene, parent.rows, parent.columns, parent.levelsBack, parent.inputs, parent.outputs);
        individual.resolveLoops();
        population.push_back(individual);
    }

    return population;
}

// goldman mutacija
vector<CGPIndividual> CGP::mutate(CGPIndividual parent) {
    vector<CGPIndividual> population;
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

        CGPIndividual individual(genes, outputGene, parent.rows, parent.columns, parent.levelsBack, parent.inputs, parent.outputs);
        individual.resolveLoops();

        //#pragma omp critical
        population.push_back(individual);
    }

    return population;

}

CGPIndividual CGP::runCGP(int generations, int rows, int columns, int levelsBack, int inputs, int outputs, int mutations) {
    vector<CGPIndividual> population;
    int bestInd = 0, generacija = 0;

    double time;
    time = omp_get_wtime();

    population = generatePopulation(rows, columns, levelsBack, inputs, outputs);

    cout << "Vrijeme: " << (omp_get_wtime() - time) << "s" << endl;

    for (generacija = 0; generacija < generations; generacija++) {
        TYPE bestFit = -2;
        bestInd = 0;
        vector<int> bestInds;
        random_device rd;
        mt19937 gen(rd());
        for (int i = 0; i < POPULATION; i++) {

            Simulator simulator;
            Bird bird;
            Controller* controller = new CGPController(population[i]);

            while (simulator.isRunning()) {
                simulator.update(bird);
                controller->action(bird, simulator);
                if (bird.distance >= MAX_MAP_SIZE)
                    break;
            }

            TYPE fit = population[i].calculateFitness((TYPE) bird.distance);
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

        cout << "Gen: " << generacija << "; Fitness: " << bestFit << "; Indeks: " << bestInd << endl;

        if (bestFit == MAX_MAP_SIZE)
            break;

        if (generacija != generations - 1)
            population = mutate(population[bestInd]);
    }

    actualGens = generacija;

    //population[bestInd].printNodes();
    ofstream outFile("CGP_best.txt");
    if (outFile.is_open()) {
        outFile << population[bestInd];
        outFile.close();
        cout << "Object written to text file." << endl;
    }

    return population[bestInd];
}

CGPIndividual CGP::CGPMain() {
    CGP cgp;
    CGPIndividual ind;


    sf::RenderWindow window(sf::VideoMode(800, 600), "Select Mode");

    sf::Font font;
    if (!font.loadFromFile(ARIAL_FONT_PATH)) {
        std::cerr << "Error loading font\n";
        return ind;
    }

    sf::Text genText("Generate New", font, 50);
    sf::FloatRect textRect1 = genText.getLocalBounds();
    genText.setOrigin(textRect1.left + textRect1.width / 2.0f,
        textRect1.top + textRect1.height / 2.0f);
    genText.setPosition(sf::Vector2f(800 / 2.0f, 600 / 2.4f));
    sf::Text runText("Run Best", font, 50);
    sf::FloatRect textRect2 = runText.getLocalBounds();
    runText.setOrigin(textRect2.left + textRect2.width / 2.0f,
        textRect2.top + textRect2.height / 2.0f);
    runText.setPosition(sf::Vector2f(800 / 2.0f, 600 / 1.6f));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (genText.getGlobalBounds().contains(mousePosF))
                    genText.setFillColor(sf::Color(250, 20, 20));
                else
                    genText.setFillColor(sf::Color(255, 255, 255));

                if (runText.getGlobalBounds().contains(mousePosF))
                    runText.setFillColor(sf::Color(250, 20, 20));
                else
                    runText.setFillColor(sf::Color(255, 255, 255));
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (genText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    ind = cgp.runCGP(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS);
                    return ind;
                }
                else if (runText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    std::ifstream inFile("CGP_best.txt");
                    if (inFile.is_open()) {
                        ind = CGPIndividual::deserialize(inFile);
                        inFile.close();
                        std::cout << "Object read from text file." << std::endl;
                        //ind.printNodes();
                        return ind;
                    }
                }
            }
        }

        window.clear();
        window.draw(genText);
        window.draw(runText);
        window.display();
    }

    return ind;
}