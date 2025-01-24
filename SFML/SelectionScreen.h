#ifndef SELECTIONSCREEN_H
#define SELECTIONSCREEN_H
#include "SFML/Graphics.hpp"
#include "Parameters.h"
#include "Controller.h"

int menu(sf::RenderWindow& window);

void trainingMenu(sf::RenderWindow& window, sf::Font& font, int generacija, int generations, float fitness, std::string model, Controller* controller);

#endif // SELECTIONSCREEN_H