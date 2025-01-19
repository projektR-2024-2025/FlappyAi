#pragma once

// TODO: premjestiti u razrede i omoguciti da se zastavice i parametri citaju iz conf datoteke

// zastavice
const bool simulationOnly = true;    // samo racuna predjenu udaljenost
const bool randomPipes = false;        // neprestano stvara nove random cijevi

// parametri prikaza (SFML koristi float)
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PIPE_WIDTH = 80;
const int BIRD_OFFSET = 80;   // polozaj ptice u odnosu na lijevi rub prozora
const int BIRD_SIZE = 40;     // velicina ptice

// parametri simulatora
const float SPEED = 0.25 * WINDOW_WIDTH; // pikseli u sekundi
const float GRAVITY = 0.85 * WINDOW_HEIGHT;
const float JUMP_SPEED = -0.5 * WINDOW_HEIGHT;
const float PIPE_GAP = 0.25 * WINDOW_HEIGHT;
