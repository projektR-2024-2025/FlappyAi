//
// Created by <3 on 07.01.2025..
//

#include "Gene.h"


int randomInt(int min, int max) {
    return min + (rand() % (int)(max - min + 1));
}

Gene::Gene() {

};

Gene::Gene(int in1, int in2)
{
    this->in1 = in1;
    this->in2 = in2;
    int index = randomInt(0, 7);  // ovo treba vidit dal daje brojeve 0 do 7
    function = static_cast<Functions>(index);
}

Gene::~Gene() {}
