#ifndef SELECTIONSCREEN_H
#define SELECTIONSCREEN_H
#include "SFML/Graphics.hpp"

enum ControllerType { NN, GP, CGP1, NONE };
enum ActionType { BEST, TRAIN };

ControllerType showSelectionScreen(sf::RenderWindow& window);

ActionType showActionScreen(sf::RenderWindow& window);

#endif // SELECTIONSCREEN_H