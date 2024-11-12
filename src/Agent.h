#ifndef AGENT_H
#define AGENT_H

class Agent {
public:
    void handleInput(char input);
    void update(int& birdPosition);
    // Add methods for AI control here

private:
    bool isJumping = false;
    int jumpStrength = 2; // How high the bird jumps
    float gravity = 0.7; // Gravity effect
};

#endif // AGENT_H