#include "Agent.h"
#include <cmath>

float velocity = 0;

void Agent::handleInput(char input) {
    if (input == ' ') {
        isJumping = true;
    }
}

void Agent::update(int& birdPosition) {
    if (isJumping) {
        birdPosition -= jumpStrength; // Move bird up
        isJumping = false;
        velocity = 0;
    }
    else {
        velocity += gravity;
        birdPosition += std::floor(velocity); // Apply gravity
    }
}