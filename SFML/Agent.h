#ifndef AGENT_H
#define AGENT_H

class Bird {
public:
    float position; // y pozicija u pikselima
    float velocity; // y brzina, pikseli u sekundi
    float distance; // prijedjena udaljenost
    bool isJumping = false;
};

#endif // AGENT_H