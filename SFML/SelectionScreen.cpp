#include "SelectionScreen.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#ifdef _WIN32
#define ARIAL_FONT_PATH "C:\\Windows\\Fonts\\arial.ttf"
#else
//#define ARIAL_FONT_PATH "arial.ttf" // ako nisi na windowsu za naseg jedinog apple usera
#define ARIAL_FONT_PATH "/System/Library/Fonts/Supplemental/Arial.ttf"
#endif

ControllerType showSelectionScreen(sf::RenderWindow& window) {
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

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (nnText.getGlobalBounds().contains(mousePosF))
                    nnText.setFillColor(sf::Color(250, 20, 20));
                else
                    nnText.setFillColor(sf::Color(255, 255, 255));

                if (gpText.getGlobalBounds().contains(mousePosF))
                    gpText.setFillColor(sf::Color(250, 20, 20));
                else
                    gpText.setFillColor(sf::Color(255, 255, 255));

                if (cgpText.getGlobalBounds().contains(mousePosF))
                    cgpText.setFillColor(sf::Color(250, 20, 20));
                else
                    cgpText.setFillColor(sf::Color(255, 255, 255));
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (nnText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    return NN;
                }
                else if (gpText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    return GP;
                }
                else if (cgpText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    return CGP1;
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

ActionType showActionScreen(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile(ARIAL_FONT_PATH)) {
        std::cerr << "Error loading font\n";
        return BEST;
    }

    sf::Text genText("Train New", font, 50);
    sf::FloatRect textRect1 = genText.getLocalBounds();
    genText.setOrigin(textRect1.left + textRect1.width / 2.0f,
        textRect1.top + textRect1.height / 2.0f);
    genText.setPosition(sf::Vector2f(800 / 2.0f, 600 / 2.4f));
    sf::Text runText("Run Best", font, 50);
    sf::FloatRect textRect2 = runText.getLocalBounds();
    runText.setOrigin(textRect2.left + textRect2.width / 2.0f,
        textRect2.top + textRect2.height / 2.0f);
    runText.setPosition(sf::Vector2f(800 / 2.0f, 600 / 1.6f));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (genText.getGlobalBounds().contains(mousePosF))
                    genText.setFillColor(sf::Color(250, 20, 20));
                else
                    genText.setFillColor(sf::Color(255, 255, 255));

                if (runText.getGlobalBounds().contains(mousePosF))
                    runText.setFillColor(sf::Color(250, 20, 20));
                else
                    runText.setFillColor(sf::Color(255, 255, 255));
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (genText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    return TRAIN;
                }
                else if (runText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    return BEST;
                }
            }
        }

        window.clear();
        window.draw(genText);
        window.draw(runText);
        window.display();
    }

    return BEST;
}