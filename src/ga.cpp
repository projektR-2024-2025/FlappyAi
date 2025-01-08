#include <iostream>
#include <thread>
#include <chrono>
#include "Simulator.h"
#include "Agent.h"
#include "Controller.h"
#include "FunctionBinaryTree.h"
#include<vector>
#include <cstdlib>  // za rand() i srand()
#include <ctime>    // za time()
#include<math.h>
#include<cmath>
#include <functional>
#include<algorithm>
#include "FitnessCalculator.h"
#include "GPcontroller.h"



using namespace std ;

//int max_ev = 100 ;
//int max_dubina = 10 ;
//int dim = 3 ;
//int vel = 100 ;

GPcontroller controller ;

struct GAParameters {
    //float crossover_rate = 0.7;
    float mutation_rate = 0.5;
    int k = 4;
    int penalty=30;
    int br = 4 ;
    float elite_percentage =0.05 ;
    float tooBad = 0.1 ;
    int max_depth = 7;

    void adapt(float diversity, float best_fitness) {
        if(diversity < 0.3) {
            mutation_rate *= 1.1;  // Increase mutation when diversity is low
            //crossover_rate *= 0.9;
        } else {
            mutation_rate = 0.5;
            //crossover_rate = 0.7;
        }
        k = max(3, min(7, int(10 * diversity)));
    }
};

GAParameters parameters ;

void resetMutation(FunctionBinaryTree& jedinka) {
    // Pick left or right subtree randomly
    if(rand() % 2 == 0 && jedinka.root->left) {
        Node* n= jedinka.root->left;
        jedinka.root->left = jedinka.createRandomNode(parameters.max_depth, 1);
        delete n ;
    } else if(jedinka.root->right) {
        delete jedinka.root->right;
        jedinka.root->right = jedinka.createRandomNode(parameters.max_depth, 1);
    }
}

float calculate_diversity(const vector<FunctionBinaryTree>& population) {
    float diversity = 0;
    for(size_t i = 0; i < population.size(); i++) {
        for(size_t j = i + 1; j < population.size(); j++) {
            FunctionBinaryTree f1 = population[i] ;
            FunctionBinaryTree f2 = population[j] ;
            if(!(f1== f2)) {
                diversity += 1;
            }
        }
    }
    return diversity / (population.size() * (population.size() - 1) / 2);
}
float update_diversity(const vector<FunctionBinaryTree>& population, FunctionBinaryTree jed, float diversity, FunctionBinaryTree other){
    diversity *= population.size() * (population.size() - 1) / 2 ;
    for(size_t i = 0; i < population.size(); i++) {
        FunctionBinaryTree f1 = population[i] ;
        if(!(f1 == jed) && (f1 == other)){
            diversity += 1;
        }        
    }
    return diversity / (population.size() * (population.size() - 1) / 2);
}

void prikaz(FunctionBinaryTree f, int br_ev, vector<FunctionBinaryTree> p){

    f.variable_names = {"yPos", "obstacle_distance", "hole_start", "hole_end"};

    Simulator simulator;
    Agent agent;
    controller.f = f ;
    
    // Main game loop
    while (simulator.isRunning() && !simulator.finishedMap()) {
        simulator.handleEvents(agent);
        controller.action(agent, simulator);
        simulator.update(agent);
        simulator.render();
        cout <<"\nevaluation: " <<br_ev <<"\n" ;
        cout <<"best function: " <<f.toString() <<endl << "best fitness: " <<f.fit <<endl ;
        cout <<"finished map: " <<simulator.finishedMap() <<endl ;
        std::this_thread::sleep_for(std::chrono::milliseconds(60)); // Control game speed
    }
    cout <<"populacija:\n" ;
    cout <<p.size() ;
    for(auto f :p){
        f.variable_names = {"yPos", "obstacle_distance", "hole_start", "hole_end"};
        cout <<f.toString()<<" fit: " <<f.fit <<endl ;
    }
     cout <<p.size() ;
    std::cout << "Press q to exit...\n";
    while (simulator.isScoreScreenActive()) {
        simulator.handleScoreEvents();
    }
    f.variable_names = {} ;
}

vector<FunctionBinaryTree> stvori_populaciju(int dim, int vel, int max_dubina){
    vector<FunctionBinaryTree> populacija ;
    for(int i = 0 ; i < vel; i++){
        FunctionBinaryTree f(max_dubina, dim) ;
        f.toString() ;
        populacija.push_back(f) ;
    }
    return populacija ;
}

float penalty(string f, int dim){
    float penalty = 0 ;
    for(int i = 0 ; i < dim ; i++){
        string s = "x" + i ;
        if(f.find(s) == string::npos){ //ne koristi se xi
            penalty += parameters.penalty ;
        }
    }
    return penalty ;
}

void evaluacija_jedinke(FunctionBinaryTree &jedinka, FunctionBinaryTree &najbolja, int dim){
    controller.f = jedinka ;
    jedinka.fit = fitness(&controller) ;
    float pen = penalty(jedinka.toString(), dim) ;
    jedinka.fit *= (1-pen/100) ;
    if(jedinka.fit > najbolja.fit)
    najbolja = jedinka ;
}

void evaluacija_populacije(vector<FunctionBinaryTree> &populacija, FunctionBinaryTree &najbolja, int dim){
    for(FunctionBinaryTree& jed: populacija){
        evaluacija_jedinke(jed, najbolja, dim) ;
    }
}

//izabere k i vraca vektor -> 0=najbolja 1=najgora
vector<int> ksel(vector<FunctionBinaryTree>& p, int k){
    int dim = p.size();
    vector<int> v;
    int best = rand() % dim;
    int worst = best;
    double bestFit = p[best].fit;
    double worstFit = bestFit;    
    for(int i = 1; i < k; i++){
        int idx = rand() % dim;
        double currentFit = p[idx].fit;
        
        if(currentFit > bestFit){
            best = idx;
            bestFit = currentFit;
        }
        if(currentFit < worstFit){
            worst = idx;
            worstFit = currentFit;
        }
    }
    v.push_back(best);
    v.push_back(worst);
    return v;
}
// Add this to your ga.cpp
void preserveElites(vector<FunctionBinaryTree>& population, int eliteCount) {
    // Find top performers without full sort (more efficient)
    vector<pair<float, int>> fitnesses;
    for(int i = 0; i < population.size(); i++) {
        fitnesses.push_back({population[i].fit, i});
    }
    
    partial_sort(fitnesses.begin(), fitnesses.begin() + eliteCount, fitnesses.end(),
        [](const auto& a, const auto& b) { return a.first > b.first; });
        
    // Store copies of elite individuals
    vector<FunctionBinaryTree> elites;
    for(int i = 0; i < eliteCount; i++) {
        elites.push_back(population[fitnesses[i].second]);
    }
    // Replace random non-elite individuals
    for(int i = 0; i < eliteCount; i++) {
        int replaceIdx = rand() % (population.size() - eliteCount) + eliteCount;
        population[replaceIdx] = elites[i];
    }
}

FunctionBinaryTree krizanje(FunctionBinaryTree r1, FunctionBinaryTree r2){
    FunctionBinaryTree dijete;
    dijete.root = r1.root->copy();
    // Randomly choose which part of the tree to crossover
    if(rand() % 2 == 0) {
        dijete.root->left = r2.root->left->copy();
    } else {
        dijete.root->right = r2.root->right->copy();
    }    
    dijete.dim = r1.dim;
    return dijete;
}

Node* changeRandomNode(Node* node, FunctionBinaryTree jed, int depth) {
    if (node == nullptr) return nullptr;    

    double mutationChance = parameters.mutation_rate + (depth * 0.1);

    mutationChance = min(mutationChance, 0.8);
    if ((rand() % 100) < (mutationChance * 100)) {
        node->value = jed.generateRandomValue(node->type);
        return node;
    }
    if (node->left) changeRandomNode(node->left, jed, depth + 1);
    if (node->right) changeRandomNode(node->right, jed, depth + 1);
    
    return node;
}

void mutacija(FunctionBinaryTree& jedinka) {
    int br = 3;    
    for(int i = 0; i < br; i++) {
        // Only mutate left and right subtrees of root, starting at depth 0
        if(rand() % 2 == 0) {
            changeRandomNode(jedinka.root->left, jedinka, 0);
        } else {
            changeRandomNode(jedinka.root->right, jedinka, 0);
        }
    }
}

void obrisi(vector<FunctionBinaryTree> &p, int r3){
    auto it = p.begin() + r3 ;
    p.erase(it) ;
}

FunctionBinaryTree ga(int dim, int vel, int max_dubina, int max_ev){

    int br_ev = 0 ;
    int elite_count= vel * parameters.elite_percentage ;

    vector<FunctionBinaryTree> populacija = stvori_populaciju(dim, vel , max_dubina) ;

    float diversity = calculate_diversity(populacija) ;
    
    FunctionBinaryTree najbolja = populacija[0];
    //cout <<"najbolja " << najbolja.toString() ;
    evaluacija_populacije(populacija, najbolja, dim) ;
    for (auto x: populacija){
        cout <<x.toString() <<" fit: " <<x.fit << endl ;
    }
    //cout <<"najbolja " << najbolja.toString() ;
    while(br_ev <= max_ev){/*
        cout <<"populacijaaa\n" ;
        for (auto x: populacija){
            cout <<x.toString() <<" fit: " <<x.fit << endl ;
        }
        cout <<"najbolja " << najbolja.toString() ;
        */
        FunctionBinaryTree r1 = populacija[ksel(populacija, parameters.k)[0]] ;
        FunctionBinaryTree r2 = populacija[ksel(populacija, parameters.k)[0]] ;
        int r3 = ksel(populacija, 2*parameters.k)[1] ; //za brisanje
        cout << "roditelj1: " << r1.toString() <<endl;
        cout << "roditelj2: " << r2.toString() <<endl;
        // cout << "za brisanje " << populacija[r3].toString() <<endl ;
        FunctionBinaryTree dijete = krizanje(r1, r2) ;
        // cout << "dijete u krizanju: " ;
        // cout <<dijete.toString() <<endl;
        mutacija(dijete) ;
        cout << "dijete u mutaciji: " ;
        cout <<dijete.toString() <<endl;
        
        evaluacija_jedinke(dijete, najbolja, dim) ;
        //cout << "roditelj1: " << r1.toString() ;
        //cout << "dijete: " ;
        //dijete.toString() ;
        //cout <<endl ;
        while(dijete.fit < najbolja.fit * parameters.tooBad){
            cout <<dijete.toString() <<" "<< dijete.fit <<endl ;
            resetMutation(dijete) ;
            evaluacija_jedinke(dijete, najbolja, dim) ;
        }
        FunctionBinaryTree temp = populacija[r3] ;
        obrisi(populacija, r3);
        populacija.push_back(dijete) ;
        br_ev++ ;
        if(br_ev % 1000 == 0){
            prikaz(najbolja, br_ev, populacija) ;
        }
        if(br_ev % 100 == 0) {
           preserveElites(populacija, elite_count);
        }

        //diversity = update_diversity(populacija, dijete, diversity, temp) ;
        //parameters.adapt(diversity, najbolja.fit) ;
        cout <<"diversity " <<diversity <<endl ;
    }
    cout <<"populacija:\n" ;
    for(auto f :populacija)cout <<f.toString()<<" fit: " <<f.fit <<endl ;
    return najbolja ;
}

int main(){
    srand(time(0));

    FunctionBinaryTree f = ga(4, 5000, 7, 10000) ;

    //prikaz(f, 5000) ;


}
