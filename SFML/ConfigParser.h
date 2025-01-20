#pragma once
#include "Parameters.h"
#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>

void parseConfig(std::string fileName) {

    std::ifstream file(fileName);
    std::map<std::string, std::string> config;

    if (!file.is_open()) {
        std::cerr << "Error opening config file!" << std::endl;
        std::cerr << "Using default config" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '='))
        {
            std::string value;
            if (std::getline(is_line, value))
                config.insert({ key, value });
        }
    }

    if(config.count("simulationOnly"))
        std::istringstream(config.find("simulationOnly")->second) >> std::boolalpha >> Parameters::simulationOnly;
    if (config.count("randomPipes"))
        std::istringstream(config.find("randomPipes")->second) >> std::boolalpha >> Parameters::randomPipes;
    if (config.count("variableFPS"))
        std::istringstream(config.find("variableFPS")->second) >> std::boolalpha >> Parameters::variableFPS;

    Parameters::WINDOW_WIDTH = config.count("WINDOW_WIDTH") ? std::stoi(config.find("WINDOW_WIDTH")->second) : Parameters::WINDOW_WIDTH;
    Parameters::WINDOW_HEIGHT = config.count("WINDOW_HEIGHT") ? std::stoi(config.find("WINDOW_HEIGHT")->second) : Parameters::WINDOW_HEIGHT;
    Parameters::PIPE_WIDTH = config.count("PIPE_WIDTH") ? std::stoi(config.find("PIPE_WIDTH")->second) : Parameters::PIPE_WIDTH;
    Parameters::BIRD_OFFSET = config.count("BIRD_OFFSET") ? std::stoi(config.find("BIRD_OFFSET")->second) : Parameters::BIRD_OFFSET;
    Parameters::BIRD_SIZE = config.count("BIRD_SIZE") ? std::stoi(config.find("BIRD_SIZE")->second) : Parameters::BIRD_SIZE;

    Parameters::SPEED = (config.count("SPEED") ? std::stof(config.find("SPEED")->second) : 0.25) * Parameters::WINDOW_WIDTH;
    Parameters::GRAVITY = (config.count("GRAVITY") ? std::stof(config.find("GRAVITY")->second) : 0.85) * Parameters::WINDOW_HEIGHT;
    Parameters::JUMP_SPEED = (config.count("JUMP_SPEED") ? std::stof(config.find("JUMP_SPEED")->second) : -0.5) * Parameters::WINDOW_HEIGHT;
    Parameters::PIPE_GAP = (config.count("PIPE_GAP") ? std::stof(config.find("PIPE_GAP")->second) : 0.3) * Parameters::WINDOW_HEIGHT;

    Parameters::FRAME_RATE = config.count("FRAME_RATE") ? std::stof(config.find("FRAME_RATE")->second) : Parameters::FRAME_RATE;

    if(config.count("MAPS")) {
        Parameters::maps.clear();
        std::string tmp;
        std::istringstream maps_line(config.find("MAPS")->second);
        while (std::getline(maps_line, tmp, ',')) {
            Parameters::maps.push_back(tmp);
        }
    }

    std::cout << "Config loaded from file" << std::endl;
}