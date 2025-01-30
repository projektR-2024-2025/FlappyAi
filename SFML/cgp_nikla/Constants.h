//
// Created by <3 on 07.01.2025..
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
    //static final int POPULATION_SIZE = 500;
    static const int POPULATION_SIZE = 100; //@@@@@@@@
    static const int ENTITY_SIZE = 200;  //@@@@@@@@number of geneSets in genome must be odd
    static const float PERCENTAGE_OF_GENES = 0.3f;
    static const float MUTATION_PROBABILITY = 0.6f;   //  [0.01, 0.05]
    static const float CROSSING_PROBABILITY = 0.7f;    //  [0.6,0.9]
    static const int RUNTIME = 600;
    static const int MIN = -50;
    static const int MAX = 150;
    static const float BETTER_PARENT_ADVANTAGE = 0.8f;
    static const int AMOUNT_OF_CGP_INPUTS = 4;
    static const int VISIBILTY_DISTANCE = 12;
    static const int DO_I_JUMP = 0;
    static const int MAX_MAP_SIZE = 10000;
    static const int NUMBER_OF_GENERATIONS = 5000;//@@@@@@@@
    //static final int NUMBER_OF_POPULATIONS_BEFORE_EVAL = 5;
}




#endif //CONSTANTS_H
