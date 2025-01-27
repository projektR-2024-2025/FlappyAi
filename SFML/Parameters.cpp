#include "Parameters.h"

bool Parameters::simulationOnly = true;
bool Parameters::randomPipes = false;
bool Parameters::variableFPS = false;

int Parameters::WINDOW_WIDTH = 800;
int Parameters::WINDOW_HEIGHT = 600;
int Parameters::PIPE_WIDTH = 80;
int Parameters::BIRD_OFFSET = 80;
int Parameters::BIRD_SIZE = 40;

float Parameters::SPEED = 0.25 * Parameters::WINDOW_WIDTH;
float Parameters::GRAVITY = 0.85 * Parameters::WINDOW_HEIGHT;
float Parameters::JUMP_SPEED = -0.5 * Parameters::WINDOW_HEIGHT;
float Parameters::PIPE_GAP = 0.30 * Parameters::WINDOW_HEIGHT;

float Parameters::FRAME_RATE = 300;

std::string map1 = "/Users/nikson/Documents/GitHub/FlappyAi/SFML/Map1.txt";//jer je out folder pokrenut
std::string map2 = "/Users/nikson/Documents/GitHub/FlappyAi/SFML/Map2.txt";;//jer je out folder pokrenut
std::string map3 = "/Users/nikson/Documents/GitHub/FlappyAi/SFML/Map3.txt";;
std::string map4 = "/Users/nikson/Documents/GitHub/FlappyAi/SFML/Map4.txt";
std::string map5 = "/Users/nikson/Documents/GitHub/FlappyAi/SFML/Map5.txt";;

std::vector<std::string> Parameters::maps({map1, map2, map3, map4, map5});


//std::vector<std::string> Parameters::maps({"Map1.txt"});


ControllerType Parameters::ctrl = NONE;
ActionType Parameters::action = BEST;

int Parameters::NUMBER_OF_EVALUATIONS = -1;