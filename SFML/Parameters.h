#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <vector>
#include <string>

enum ControllerType { NN, GP1, GP2, CGP1, CGP2, NONE };
enum ActionType { BEST, TRAIN };

class Parameters
{
public:
	// zastavice
	static bool simulationOnly;    // samo racuna predjenu udaljenost
	static bool randomPipes;        // neprestano stvara nove random cijevi
	static bool variableFPS;

	// parametri prikaza (SFML koristi float)
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static int PIPE_WIDTH;
	static int BIRD_OFFSET;   // polozaj ptice u odnosu na lijevi rub prozora
	static int BIRD_SIZE;     // velicina ptice

	// parametri simulatora
	static float SPEED; // pikseli u sekundi
	static float GRAVITY;
	static float JUMP_SPEED;
	static float PIPE_GAP;

	static float FRAME_RATE;

	static std::vector<std::string> maps;

	static ControllerType ctrl;
	static ActionType action;
};

#endif
