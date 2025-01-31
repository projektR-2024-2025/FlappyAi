#include "../Controller.h"
#include "GA.h"
#include "TreeParser.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include "../LiberationSansFont.h"

using namespace std;
using namespace tonka;



#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || __cplusplus >= 202002L)
GAParameters gaParams = {
    .mutation_rate = 0.5,
    .k = 4,
    .penalty = 30,
    .br = 7,
    .elite_percentage = 0.05,
    .tooBad = 0.1,
    .max_depth = 15
    .crossover_rate
};
#else
GAParameters gaParams = { 0.5, 4, 30, 7, 0.05, 0.1, 15, 0.95 };
#endif

void GAParameters::adapt(float diversity, float best_fitness) {
    if(diversity < 0.3) {
        mutation_rate *= 1.1;  // Increase mutation when diversity is low
    } else {
        mutation_rate = 0.5;
    }
    k = max(3, min(7, int(10 * diversity)));
}

void resetMutation(FunctionBinaryTree& jedinka) {
    if(myrand() % 2 == 0 && jedinka.root->left) {
        tonka::NodeT* n = jedinka.root->left;
        jedinka.root->left = jedinka.createRandomNode(gaParams.max_depth, 1);
        delete n;
    } else if(jedinka.root->right) {
        tonka::NodeT* n = jedinka.root->right;
        jedinka.root->right = jedinka.createRandomNode(gaParams.max_depth, 1);
        delete n;
    }
}

float calculate_diversity(const vector<FunctionBinaryTree>& population) {
    float diversity = 0;
    for(size_t i = 0; i < population.size(); i++) {
        for(size_t j = i + 1; j < population.size(); j++) {
            FunctionBinaryTree f1 = population[i];
            FunctionBinaryTree f2 = population[j];
            if(!(f1 == f2)) {
                diversity += 1;
            }
        }
    }
    return diversity / (population.size() * (population.size() - 1) / 2);
}

vector<FunctionBinaryTree> stvori_populaciju(int dim, int vel, int max_dubina) {
    vector<FunctionBinaryTree> populacija;
    for(int i = 0; i < vel; i++) {
        FunctionBinaryTree f(max_dubina, dim);
        populacija.push_back(f);
    }
    return populacija;
}

float penalty(string f, int dim) {
    float penalty = 0;
    for(int i = 0; i < dim; i++) {
        string s = "x" + to_string(i);
        if(f.find(s) == string::npos) {
            penalty += gaParams.penalty;
        }
    }
    return penalty;
}

void evaluacija_jedinke(FunctionBinaryTree& jedinka, FunctionBinaryTree& najbolja, int dim) {
    jedinka.fitness();
    float pen = penalty(jedinka.toString(), dim);
    //jedinka.fit *= (1 - pen/100);
    if(jedinka.fit > najbolja.fit)
        najbolja = jedinka;
}

void evaluacija_populacije(vector<FunctionBinaryTree>& populacija, FunctionBinaryTree& najbolja, int dim) {
    int i = 0 ;
    for(FunctionBinaryTree& jed: populacija) {
        i++ ;
        evaluacija_jedinke(jed, najbolja, dim);
        cout << "evaluacija... " <<i <<endl ;
    }
}

vector<int> ksel(vector<FunctionBinaryTree>& p, int k) {
    int dim = p.size();
    
    // Get first random individual
    int idx = myrand() % dim;
    int best = idx;
    int worst = idx;
    double bestFit = p[idx].fit;
    double worstFit = p[idx].fit;
    
    //cout << "Initial selection: idx=" << idx << " fitness=" << bestFit << endl;
    
    // Start tournament from 1 since we already used first selection
    for(int i = 1; i < k; i++) {
        idx = myrand() % dim;
        double currentFit = p[idx].fit;
        
        //cout << "Tournament round " << i << ": idx=" << idx << " fitness=" << currentFit << endl;
        
        if(currentFit > bestFit) {
            best = idx;
            bestFit = currentFit;
        }
        if(currentFit < worstFit) {
            worst = idx;
            worstFit = currentFit;
        }
        
        //cout << "Current best=" << best << " (fitness " << bestFit << ")" << endl;
        //cout << "Current worst=" << worst << " (fitness " << worstFit << ")" << endl;
    }

  
    return {best, worst};
}

void preserveElites(vector<FunctionBinaryTree>& population, int eliteCount) {
    vector<pair<float, int>> fitnesses;
    for(int i = 0; i < population.size(); i++) {
        fitnesses.push_back({population[i].fit, i});
    }
    
    partial_sort(fitnesses.begin(), fitnesses.begin() + eliteCount, fitnesses.end(),
        [](const auto& a, const auto& b) { return a.first > b.first; });
        
    vector<FunctionBinaryTree> elites;
    for(int i = 0; i < eliteCount; i++) {
        elites.push_back(population[fitnesses[i].second]);
    }
    
    for(int i = 0; i < eliteCount; i++) {
        int replaceIdx = myrand() % (population.size() - eliteCount) + eliteCount;
        population[replaceIdx] = elites[i];
    }
}

FunctionBinaryTree krizanje(FunctionBinaryTree r1, FunctionBinaryTree r2) {
    FunctionBinaryTree dijete;
    dijete.root = r1.root->copy();
    if(myrand() % 2 == 0) {
        dijete.root->left = r2.root->left->copy();
    } else {
        dijete.root->right = r2.root->right->copy();
    }    
    dijete.dim = r1.dim;
    return dijete;
}

tonka::NodeT* changeRandomNode(tonka::NodeT* node, FunctionBinaryTree jed, int depth) {
    if (node == nullptr) return nullptr;    

    double mutationChance = gaParams.mutation_rate + (depth * 0.1);
    mutationChance = min(mutationChance, 0.8);
    
    if ((myrand() % 100) < (mutationChance * 100)) {
        node->value = jed.generateRandomValue(node->type);
        return node;
    }
    
    if (node->left) changeRandomNode(node->left, jed, depth + 1);
    if (node->right) changeRandomNode(node->right, jed, depth + 1);
    
    return node;
}

void mutacija(FunctionBinaryTree& jedinka) {
    int br = gaParams.br;    
    for(int i = 0; i < br; i++) {
        if(myrand() % 2 == 0) {
            changeRandomNode(jedinka.root->left, jedinka, 0);
        } else {
            changeRandomNode(jedinka.root->right, jedinka, 0);
        }
    }
}

void obrisi(vector<FunctionBinaryTree>& p, int r3) {
    auto it = p.begin() + r3;
    p.erase(it);
}

FunctionBinaryTree ga(int dim, int vel, int max_dubina, int max_ev, sf::RenderWindow& window) {
    int br_ev = 0;

    srand(time(0));

    vector<FunctionBinaryTree> populacija = stvori_populaciju(dim, vel, max_dubina);
    for (auto f: populacija){
        cout << f.toString() <<endl ;
    }

    sf::Font font;
    font.loadFromMemory(&LiberationSans_Regular_ttf, LiberationSans_Regular_ttf_len);
    
    FunctionBinaryTree najbolja = populacija[0];
    evaluacija_populacije(populacija, najbolja, dim);
    
    while(br_ev <= max_ev) {
        FunctionBinaryTree r1 = populacija[ksel(populacija, gaParams.k)[0]];
        //FunctionBinaryTree r1 = najbolja;
        FunctionBinaryTree r2 = populacija[ksel(populacija, gaParams.k)[0]];
        int r3 = ksel(populacija, 2*gaParams.k)[1];
        FunctionBinaryTree dijete ;
        if((static_cast<float>(myrand()) / RAND_MAX) < gaParams.mutation_rate)
            dijete = krizanje(r1, r2);
        else{
            dijete.root = r1.root->copy();
            dijete.dim = r1.dim;
        }
        if((static_cast<float>(myrand()) / RAND_MAX) < gaParams.mutation_rate)
            mutacija(dijete);
        evaluacija_jedinke(dijete, najbolja, dim);
        
        cout <<"r1: " << r1.toString() << r1.fit<< endl ;
        cout <<"r2: " << r2.toString() <<  r2.fit <<endl ;
        cout <<"r3: " << populacija[r3].toString() <<  populacija[r3].fit << endl ;
        cout <<"dijete: " << dijete.toString() <<dijete.fit << endl ;

        obrisi(populacija, r3);
        populacija.push_back(dijete);
        //populacija[r3]=dijete;
        br_ev++;

        // std::vector<float> F;
        // std::map<std::string, int> M;
        // using pair_type = decltype(M)::value_type;
        // for (auto f: populacija){
        //     M[f.toString()]++;
        //     F.push_back(f.fit);
        //    // cout << f.toString() <<endl ;
        // }
        // std::sort(F.begin(), F.end());
        // int maxrep = std::max_element(M.begin(), M.end(),
        //   [] (pair_type& p1, pair_type& p2) {
        //     return p1.second < p2.second;
        //   })->second;
        // int sumrep = std::accumulate(M.begin(), M.end(), 0,
        //   [](std::size_t previous, pair_type& p) {
        //     return previous + p.second; }
        //   );

        // cout <<"Populacija max=" <<F.back()
        //      <<" min=" <<F.front() 
        //      <<" med=" <<F[F.size()/2]
        //      <<" maxrep=" <<maxrep
        //      <<" avgrep=" <<F.size()/(float)M.size()
        //      <<"\n";

        if (Parameters::NUMBER_OF_EVALUATIONS > 0 && br_ev * Parameters::maps.size() > Parameters::NUMBER_OF_EVALUATIONS)
            break;

        if (Parameters::NUMBER_OF_EVALUATIONS > 0)
            trainingMenu(window, font, br_ev * Parameters::maps.size(), Parameters::NUMBER_OF_EVALUATIONS, najbolja.fit, "GP", new GPcontroller(najbolja));
        else
            trainingMenu(window, font, br_ev, max_ev, najbolja.fit, "GP", new GPcontroller(najbolja));

        cout <<"evaluacija: " <<br_ev <<" najbolja: "<< najbolja.toString() <<" fit: " <<najbolja.fit <<endl <<endl ;
    }
    for (auto a: populacija){
        //cout << a.toString() << " fit: " <<a.fit <<endl ;
    }
    try {
        saveBestToFile(najbolja);
    } catch (const exception& e) {
        cerr << "Error saving best GP: " << e.what() << endl;
    }
    return najbolja;
}

FunctionBinaryTree loadBestFromFile() {
    std::ifstream file("best_gp.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Could not open best_gp.txt");
    }
    
    std::string expression;
    std::getline(file, expression);
    file.close();
    FunctionBinaryTree tree = TreeParser::fromString(expression, 4) ;
    return tree;
}

void saveBestToFile(FunctionBinaryTree best) {
    std::ofstream file("best_gp.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Could not open best_gp.txt for writing");
    }
    string s = best.toString() ;
    file << s;
    file.close();
}

Controller* GPMain(sf::RenderWindow& window) {
    if (Parameters::action == BEST) {
        try {
            FunctionBinaryTree bestTree = loadBestFromFile();
            return new GPcontroller(bestTree);
        } catch (const std::exception& e) {
            std::cerr << "Error loading best GP: " << e.what() << std::endl;
            Parameters::action = TRAIN;
        }
    }
    
    if (Parameters::action == TRAIN) {
        const int POPULATION_SIZE = 5000;
        const int MAX_EVALUATIONS = 50000;
        const int MAX_DEPTH = gaParams.max_depth;
        const int INPUT_DIM = 4; // yPos, obstacle_distance, hole_start, hole_end
        
        FunctionBinaryTree bestIndividual = ga(INPUT_DIM, POPULATION_SIZE, MAX_DEPTH, MAX_EVALUATIONS, window);
        
        try {
            FunctionBinaryTree bestTree = loadBestFromFile();
            bestTree.fitness() ;
            if(bestIndividual.fit > bestTree.fit){
                saveBestToFile(bestIndividual);
            }
        } catch (const exception& e) {
            cerr << "Error saving best GP: " << e.what() << endl;
        }
        
        return new GPcontroller(bestIndividual);
    }
    
    throw std::runtime_error("Invalid action type for GP");
}