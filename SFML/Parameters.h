#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <vector>
#include <string>

/**
 * Enumeracija koja predstavlja sve moguce tipove kontrolera.
 */
enum ControllerType { NN, GP1, GP2, CGP1, CGP2, MANUAL, NONE };
/**
 * Enumeracija koja predstavlja sve moguce tipove akcija.
 */
enum ActionType { BEST, TRAIN };

/**
 * Klasa koja predstavlja parametre.
 */
class Parameters
{
public:
	/**
	 * Hoce li se simulator prikazivati u SFML prozoru ili samo vrtiti u pozadini.
	 */
	static bool simulationOnly;
	/**
	 * Hoce li se cijvi nasumicno generirati ili ucitati iz mape.
	 */
	static bool randomPipes;
	/**
	 * Ako true, pri ucenju ce se koristiti 1 / @ref FRAME_RATE +- 10% za deltaTime, a pri pokretanju proteklo vrijeme na clocku.<br>
	 * Ako false, deltaTime je konstantan na 1 / @ref FRAME_RATE.
	 */
	static bool variableFPS;

	/**
	 * Sirina SFML prozora.
	 */
	static int WINDOW_WIDTH;
	/**
	 * Visina SFML prozora.
	 */
	static int WINDOW_HEIGHT;
	/**
	 * Sirina cijevi.
	 */
	static int PIPE_WIDTH;
	/**
	 * Polozaj ptice u odnosu na lijevi rub prozora. (lijevog brida ptice)
	 */
	static int BIRD_OFFSET;
	/**
	 * Velicina ptice.
	 */
	static int BIRD_SIZE;

	/**
	 * Brzina ptice na horizontalnoj osi. (pikseli / sekundi)
	 */
	static float SPEED;
	/**
	 * Akceleracija prema dolje. (pikseli / sekundi^2)
	 */
	static float GRAVITY;
	/**
	 * Brzina kojom ptica skoci u zrak. (pikseli / sekundi)
	 */
	static float JUMP_SPEED;
	/**
	 * Razmak izmedu gornjeg i donjeg dijela cijevi. (postotak @ref WINDOW_HEIGHT)
	 */
	static float PIPE_GAP;

	/**
	 * Broj okvira po sekundi koji ce se prikazati.
	 */
	static float FRAME_RATE;

	/**
	 * Mape koje ce se koristiti za ucenje i pokretanje simulatora.
	 */
	static std::vector<std::string> maps;

	/**
	 * Odabrani tip kontrolera.
	 */
	static ControllerType ctrl;
	/**
	 * Odabrani tip akcije.
	 */
	static ActionType action;

	/**
	 * Na koliko evaluacija zelimo da se modeli uce.
	 */
	static int NUMBER_OF_EVALUATIONS;
};

#endif
