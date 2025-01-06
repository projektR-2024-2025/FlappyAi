#include "SelectionScreen.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#ifdef _WIN32
#define ARIAL_FONT_PATH "C:\\Windows\\Fonts\\arial.ttf"
#else
#define ARIAL_FONT_PATH "arial.ttf" // ako nisi na windowsu za naseg jedinog apple usera
#endif

ControllerType showSelectionScreen() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Select Controller");

    sf::Font font;
    if (!font.loadFromFile(ARIAL_FONT_PATH)) {
        std::cerr << "Error loading font\n";
        return NONE;
    }

    sf::Text nnText("NN", font, 50);
    nnText.setPosition(100, 250);
    sf::Text gpText("GP", font, 50);
    gpText.setPosition(350, 250);
    sf::Text cgpText("CGP", font, 50);
    cgpText.setPosition(600, 250);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (nnText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    return NN;
                }
                else if (gpText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    return GP;
                }
                else if (cgpText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    return CGP;
                }
            }
        }

        window.clear();
        window.draw(nnText);
        window.draw(gpText);
        window.draw(cgpText);
        window.display();
    }

    return NONE;
}