#ifndef SELECTIONSCREEN_H
#define SELECTIONSCREEN_H
#include "SFML/Graphics.hpp"
#include "Parameters.h"
#include "Controller.h"

/**
 * Glavni meni iz kojeg se dalje navigira.
 */
int menu(sf::RenderWindow& window);

/**
 * Ekran koji se prikazuje prilikom ucenja modela.
 */
void trainingMenu(sf::RenderWindow& window, sf::Font& font, int generacija, int generations, float fitness, std::string model, Controller* controller);

#endif // SELECTIONSCREEN_H