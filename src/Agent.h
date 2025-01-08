#ifndef AGENT_H
#define AGENT_H

class Agent {
public:
    void handleInput(char input);
    double update(int& birdPosition);
    // Add methods for AI control here 
    void changeGameSpeed(int acceleration){
        jumpStrength *= acceleration ; 
        gravity *= acceleration ;
        maxVelocity *= acceleration ;
    }

    double velocity;
    double position;    // domain: [0, 1] [top, bottom]
    bool isJumping = false;

    Agent()
    {
        velocity = 0;
        position = 0.5;
    }

private:
    double jumpStrength = 0.15; // How powerfull the bird jumps
    double gravity = 0.05; // Gravity effect
    double maxVelocity = 0.05; // max downward velocity
};

#endif // AGENT_H