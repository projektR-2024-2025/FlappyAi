#include "SelectionScreen.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Parameters.h"

#ifdef _WIN32
#define ARIAL_FONT_PATH "C:\\Windows\\Fonts\\arial.ttf"
#elif __linux__
#define ARIAL_FONT_PATH "/usr/share/fonts/liberation/LiberationSans-Regular.ttf" 
#else
#define ARIAL_FONT_PATH "arial.ttf" // ako nisi na windowsu za naseg jedinog apple usera
#endif

struct MenuOption {
    sf::Text text;
    bool selected;
};

void setupMenu(std::vector<MenuOption>& menu, sf::Font& font, const std::vector<std::string>& options) {
    float spacing = 70.f;
    float totalHeight = options.size() * spacing;
    float yOffset = (Parameters::WINDOW_HEIGHT - totalHeight) / 2.f;

    for (size_t i = 0; i < options.size(); ++i) {
        MenuOption option;
        option.text.setFont(font);
        option.text.setString(options[i]);
        option.text.setCharacterSize(36);
        option.text.setFillColor(sf::Color::White);
        option.text.setPosition(Parameters::WINDOW_WIDTH / 2.f - option.text.getLocalBounds().width / 2.f, yOffset + i * spacing);
        option.selected = false;
        menu.push_back(option);
    }
    menu[0].selected = true;
}

void updateMenu(std::vector<MenuOption>& menu, int selectedIndex) {
    for (size_t i = 0; i < menu.size(); ++i) {
        if (i == selectedIndex) {
            menu[i].text.setFillColor(sf::Color(0, 184, 255));
        }
        else {
            menu[i].text.setFillColor(sf::Color::White);
        }
    }
}

int showMenu(sf::RenderWindow& window, std::vector<std::string> optionNames) {
    sf::Font font;
    if (!font.loadFromFile(ARIAL_FONT_PATH)) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    std::vector<MenuOption> menu;
    setupMenu(menu, font, optionNames);

    int selectedIndex = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedIndex = (selectedIndex > 0) ? selectedIndex - 1 : menu.size() - 1;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedIndex = (selectedIndex + 1) % menu.size();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    return selectedIndex;
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                for (size_t i = 0; i < menu.size(); ++i) {
                    if (menu[i].text.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                        selectedIndex = i;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < menu.size(); ++i) {
                    if (menu[i].text.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        return i;
                    }
                }
            }
        }

        updateMenu(menu, selectedIndex);

        window.clear(sf::Color(50, 50, 50));
        for (const auto& option : menu) {
            window.draw(option.text);
        }
        window.display();
    }

    return -1;
}

void menu(sf::RenderWindow& window) {
    std::vector<std::string> optionNames = { "Start Game", "Exit" };
    int option;
    option = showMenu(window, optionNames);

    if (option == 1) {
        window.close();
        return;
    }

    optionNames = { "GP1", "GP2", "CGP1", "CGP2", "NN" };
    std::vector<ControllerType> controllers = { GP1, GP2, CGP1, CGP2, NN };
    option = showMenu(window, optionNames);
    Parameters::ctrl = controllers[option];

    optionNames = { "Train new individual", "Run best individual"};
    std::vector<ActionType> actions = { TRAIN, BEST };
    option = showMenu(window, optionNames);
    Parameters::action = actions[option];
}