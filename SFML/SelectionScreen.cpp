#include "SelectionScreen.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Parameters.h"
#include "LiberationSansFont.h"
#include "Controller.h"

struct MenuOption {
    sf::Text text;
    bool selected;
};

void setupMenu(std::vector<MenuOption>& menu, sf::Font& font, const std::vector<std::string>& options) {
    float spacing = 70.f;
    float totalHeight = options.size() * spacing;
    float yOffset = (Parameters::WINDOW_HEIGHT - totalHeight) / 2.f;

    for (size_t i = 0; i < options.size() && i < 5; ++i) {
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

int showMenu(sf::RenderWindow& window, std::vector<std::string> optionNames, bool exit) {
    sf::Font font;
    font.loadFromMemory(&LiberationSans_Regular_ttf, LiberationSans_Regular_ttf_len);

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
                else if (event.key.code == sf::Keyboard::Escape && exit) {
                    return 2;
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

int mapSelectorMenu(sf::RenderWindow& window) {
    sort(Parameters::maps.begin(), Parameters::maps.end());
    int option = showMenu(window, Parameters::maps, false);
    std::string tmpMap(Parameters::maps.at(option));
    Parameters::maps.at(option) = Parameters::maps.at(0);
    Parameters::maps.at(0) = tmpMap;

    return 0;
}

int startMenu(sf::RenderWindow& window) {
    while (window.isOpen()) {
        std::vector<std::string> optionNames = { "Start Game", "Map Selector", "Exit" };
        int option;
        option = showMenu(window, optionNames, true);

        if (option == 1) {
            mapSelectorMenu(window);
            continue;
        }

        if (option == 2) {
            window.close();
            return -1;
        }
        return 0;
    }
    return 0;
}

int selectionMenu(sf::RenderWindow& window) {
    std::vector<std::string> optionNames = { "Play By Yourself", "Train New Individual", "Run Best Individual" };
    std::vector<ActionType> actions = { BEST, TRAIN, BEST };
    int option = showMenu(window, optionNames, false);
    Parameters::action = actions[option];

    if (option == 0) {
        Parameters::ctrl = MANUAL;
        return 1;
    }
    return 0;
}

int actionMenu(sf::RenderWindow& window) {
    std::vector<std::string> optionNames = { "GP1", "GP2", "CGP1", "CGP2", "NN" };
    std::vector<ControllerType> controllers = { GP1, GP2, CGP1, CGP2, NN };
    int option = showMenu(window, optionNames, false);
    Parameters::ctrl = controllers[option];
    
    return 0;
}

int menu(sf::RenderWindow& window) {
    
    if (startMenu(window) == -1)
        return -1;

    if (selectionMenu(window) == 0)
        actionMenu(window);

    return 0;
}

void trainingMenu(sf::RenderWindow& window, sf::Font& font, int generacija, int generations, float fitness, std::string model, Controller* controller) {
    if (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color(50, 50, 50));

        if (generacija % (int)(generations * 0.1) != 0){
            sf::RectangleShape prog1;
            sf::RectangleShape prog2;

            prog1.setPosition(Parameters::WINDOW_WIDTH / 2.f - Parameters::WINDOW_WIDTH * 0.9f / 2.f, Parameters::WINDOW_HEIGHT / 2.f - Parameters::WINDOW_HEIGHT * 0.1f / 2.f);
            prog1.setSize(sf::Vector2f(Parameters::WINDOW_WIDTH * 0.9f * ((float)generacija / (float)generations), Parameters::WINDOW_HEIGHT * 0.1f));
            prog1.setFillColor(sf::Color(0, 184, 255));
            prog2.setPosition(Parameters::WINDOW_WIDTH / 2.f - Parameters::WINDOW_WIDTH * 0.9f / 2.f, Parameters::WINDOW_HEIGHT / 2.f - Parameters::WINDOW_HEIGHT * 0.1f / 2.f);
            prog2.setSize(sf::Vector2f(Parameters::WINDOW_WIDTH * 0.9f, Parameters::WINDOW_HEIGHT * 0.1f));
            prog2.setFillColor(sf::Color(69, 69, 69));

            window.draw(prog2);
            window.draw(prog1);

            float spacing = 150.f;
            std::string prog = std::to_string((float)generacija / (float)generations * 100.f);
            prog.erase(prog.end() - 4, prog.end());
            std::string fit = std::to_string(fitness);
            fit.erase(fit.end() - 3, fit.end());
            std::vector<std::string> textOptions = { "Training with " + model + "...", prog + "%", "Best Fitness: " + fit};
            for (int i = 0; i < 3; i++) {
                sf::Text text;
                text.setFont(font);
                text.setString(textOptions[i]);
                text.setCharacterSize(36);
                text.setFillColor(sf::Color::White);
                text.setPosition(Parameters::WINDOW_WIDTH / 2.f - text.getLocalBounds().width / 2.f, (Parameters::WINDOW_HEIGHT - (textOptions.size() - 1) * spacing - textOptions.size() * text.getLocalBounds().height) / 2.f + i * spacing + text.getLocalBounds().height / 2.f);
                window.draw(text);
            }
        }
        else if (generacija < generations) {
            Parameters::simulationOnly = false;

            Simulator simulator(&window);
            Bird agent;
            simulator.initialize(agent);

            // Main game loop
            while (simulator.isRunning()) {
                simulator.update(agent);
                controller->action(agent, simulator);
            }

            Parameters::simulationOnly = true;
        }

        window.display();
    }
    else {
        Parameters::simulationOnly = false;
    }
}