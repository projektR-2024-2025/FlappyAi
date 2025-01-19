//
// Created by <3 on 07.01.2025..
//

#ifndef GENE_H
#define GENE_H
#include <random>
#include <cmath>


int randomInt(int min, int maxExclusive);

enum class Functions {
    PLUS, MINUS, MUL, DIV, MOD, COS, SIN, POW
};

class Gene {
public:
    int in1;
    int in2;
    Functions function;

    Gene(int in1, int in2);
    Gene();
};



#endif //GENE_H
