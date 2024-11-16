#ifndef AGENT_H
#define AGENT_H

class Agent {
public:
    void handleInput(char input);
    double update(int& birdPosition);
    // Add methods for AI control here

    double velocity;
    double position;    // domain: [0, 1] [top, bottom]

    Agent()
    {
        velocity = 0;
        position = 0.5;
    }

private:
    bool isJumping = false;
    double jumpStrength = 0.15; // How high the bird jumps
    double gravity = 0.05; // Gravity effect
    double maxVelocity = 0.15; // max downward velocity
};

#endif // AGENT_H