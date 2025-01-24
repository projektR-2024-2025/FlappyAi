#include "Parameters.h"

bool Parameters::simulationOnly = true;
bool Parameters::randomPipes = false;
bool Parameters::variableFPS = false;

// parametri prikaza (SFML koristi float)
int Parameters::WINDOW_WIDTH = 800;
int Parameters::WINDOW_HEIGHT = 600;
int Parameters::PIPE_WIDTH = 80;
int Parameters::BIRD_OFFSET = 80;   // polozaj ptice u odnosu na lijevi rub prozora
int Parameters::BIRD_SIZE = 40;     // velicina ptice

// parametri simulatora
float Parameters::SPEED = 0.25 * Parameters::WINDOW_WIDTH; // pikseli u sekundi
float Parameters::GRAVITY = 0.85 * Parameters::WINDOW_HEIGHT;
float Parameters::JUMP_SPEED = -0.5 * Parameters::WINDOW_HEIGHT;
float Parameters::PIPE_GAP = 0.30 * Parameters::WINDOW_HEIGHT;

float Parameters::FRAME_RATE = 300;

std::vector<std::string> Parameters::maps({"Map1.txt"});

ControllerType Parameters::ctrl = NONE;
ActionType Parameters::action = BEST;

int Parameters::NUMBER_OF_EVALUATIONS = -1;