#include "Agent.h"
#include <cmath>

void Agent::handleInput(char input) {
    if (input == ' ') {
        isJumping = true;
    }
}

// returns new position
double Agent::update(int& birdPosition) {
    if (isJumping) {
        //position -= jumpStrength; // option a) move bird up
        velocity = -jumpStrength;   // option b) set upwards velocity (like in the original)
        //velocity -= jumpStrength;   // option c) increase upwards velocity component

        isJumping = false;
    }
    //else 
    {
        velocity += gravity;    // Apply gravity
        if (velocity > maxVelocity)
            velocity = maxVelocity;
        position += velocity;
    }
    return position;
}