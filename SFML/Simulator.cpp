#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include "Parameters.h"


Simulator::Simulator() : running(true)
{}

Simulator::Simulator(sf::RenderWindow* window) : running(true), window(window)
{}

Simulator::Simulator(std::string map) : running(true), map(map)
{}

bool Simulator::initialize(Bird& bird) 
{
    srand(static_cast<unsigned>(time(0)));

    // idemo li dalje
    running = true;

    // ptica, cijevi
    std::vector<Pipe> pipes;
    initializeMap(bird);

    // izgled ptice
    // TODO: stavite neki sprite
    birdShape.setSize(sf::Vector2f(Parameters::BIRD_SIZE, Parameters::BIRD_SIZE));
    birdShape.setFillColor(sf::Color::Yellow);

    // izgled cijevi
    // TODO: sprite
    topPipe.setSize(sf::Vector2f(Parameters::PIPE_WIDTH, Parameters::WINDOW_HEIGHT));
    topPipe.setFillColor(sf::Color::Green);
    bottomPipe.setSize(sf::Vector2f(Parameters::PIPE_WIDTH, Parameters::WINDOW_HEIGHT));
    bottomPipe.setFillColor(sf::Color::Green);

    // tlo na 50 piksela od dna prozora
    ground.setSize(sf::Vector2f(Parameters::WINDOW_WIDTH, 50));
    ground.setFillColor(sf::Color::Green);
    ground.setPosition(0, Parameters::WINDOW_HEIGHT - 50);

    clock.restart();

    return true;
}


bool Simulator::isRunning() const 
{
    if (Parameters::simulationOnly)
        return running;
    else if (Parameters::action == TRAIN && Parameters::simulationOnly == false)
        return running;
    return true;
}


bool Simulator::simulateFrame(Bird& bird, float deltaTime) 
{
    // brzina i visina
    bird.velocity += Parameters::GRAVITY * deltaTime;
    bird.position += bird.velocity * deltaTime;

    // pomakni cijevi, provjeri sudar
    for (auto& pipe : pipes) {
        pipe.x -= Parameters::SPEED * deltaTime;

        // je li sudar
        if (pipe.x < (Parameters::BIRD_OFFSET + Parameters::BIRD_SIZE) && (pipe.x + Parameters::PIPE_WIDTH) > Parameters::BIRD_OFFSET) { // presjek po sirini
            if (bird.position < pipe.topY || bird.position + Parameters::BIRD_SIZE > pipe.bottomY) { // presjek po visini
                return false; // Collision detected
            }
        }

        if (Parameters::randomPipes) {
            // stvori slucajnu novu cijev kad ovu predjemo
            if (pipe.x + Parameters::PIPE_WIDTH < 0) {
                float pipeHeight = (float)(rand() % 200 + 100);
                pipe.x = pipes.size() * 300 + pipes.size() - 1 * Parameters::PIPE_WIDTH;
                pipe.topY = pipeHeight;
                pipe.bottomY = pipeHeight + Parameters::PIPE_GAP;
            }
        }
        else if(Parameters::simulationOnly) {
            if (pipe.x + Parameters::PIPE_WIDTH < 0) {
                pipes.erase(pipes.begin());
            }
        }
    }

    // je li sudar s tlom ili vrhom prozora
    if (bird.position < 0 || bird.position + Parameters::BIRD_SIZE > Parameters::WINDOW_HEIGHT - 50) {
        return false;
    }

    // koliko smo prosli
    bird.distance += Parameters::SPEED * deltaTime;

    return true; // nema sudara
}


void Simulator::update(Bird& bird) 
{
    if (Parameters::simulationOnly || window->isOpen()) {
        if (!Parameters::simulationOnly) {
            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window->close();
                    running = false;
                    Parameters::simulationOnly = true;
                }
                // space za skok
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                    bird.velocity = Parameters::JUMP_SPEED;
                }
                // 'r' za restart
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    initializeMap(bird);
                    running = true;
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    running = false;
                    Parameters::simulationOnly = true;
                }
            }
            if (!Parameters::randomPipes && pipes.at(pipes.size() - 1).x + Parameters::PIPE_WIDTH < 0) {
                running = false;
                Parameters::simulationOnly = true;
            }
        }

        // najvaznija varijabla: period simulacije
        float deltaTime;
        if(Parameters::variableFPS) {
            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_real_distribution<> deltaDis(-((1.f / Parameters::FRAME_RATE) / 10.f), ((1.f / Parameters::FRAME_RATE) / 10.f));

            if (Parameters::simulationOnly)
                deltaTime = (1.f / Parameters::FRAME_RATE) + deltaDis(gen);  // uzmimo kao da su prosle 3 stotinke (simulirano vrijeme)
            else
                deltaTime = clock.restart().asSeconds();    // koliko je proslo ako koristimo prikaz (real time)
        }
        else
            deltaTime = (1.f / Parameters::FRAME_RATE);

        // korak simulacije
        if (running && !simulateFrame(bird, deltaTime)) {
            //std::cout << "Prijedjena udaljenost: " << bird.distance << "\n";
            running = false;
            //resetGame(bird, pipes, distance);
            if (Parameters::simulationOnly) {
                return; // nema reseta ako nema prikaza
            }
        }

        // ako nije simulacija, ovdje se sve crta
        if (!Parameters::simulationOnly) {
            window->clear(sf::Color(135, 206, 250)); // nebo
            window->draw(ground);

            // crtaj pticu
            birdShape.setPosition(Parameters::BIRD_OFFSET, bird.position);
            window->draw(birdShape);

            // crtaj cijevi - samo prve 4
            for (int i = 0; i < pipes.size(); i++) {
                topPipe.setPosition(pipes[i].x, 0);
                topPipe.setSize(sf::Vector2f(Parameters::PIPE_WIDTH, pipes[i].topY));
                bottomPipe.setPosition(pipes[i].x, pipes[i].bottomY);
                bottomPipe.setSize(sf::Vector2f(Parameters::PIPE_WIDTH, Parameters::WINDOW_HEIGHT - pipes[i].bottomY));
                topPipe.setFillColor(sf::Color::Blue);
                bottomPipe.setFillColor(sf::Color::Blue);

                window->draw(topPipe);
                window->draw(bottomPipe);
            }

            window->display();
        }
    }

}


void Simulator::initializeMap(Bird& bird) 
{
    // pocetna pozicija
    bird.position = Parameters::WINDOW_HEIGHT / 2;
    bird.velocity = 0.0;
    bird.distance = 0.0;

    // nove cijevi
    pipes.clear();

    // a) slucajne cijevi
    if (Parameters::randomPipes)
        for (float x = Parameters::WINDOW_WIDTH * 0.75; x < 2 * Parameters::WINDOW_WIDTH; x += 300) {
            float pipeHeight = (float) (rand() % 200 + 100);
            pipes.push_back({ x, pipeHeight, pipeHeight + Parameters::PIPE_GAP });
        }
    // b) hard coded ili ucitane cijevi
    else {
        // cijevi su dimenzionirane relativno na velicinu prozora, [0, 1]
        //pipes = std::vector<Pipe>{ { 0.5, 0.3, 0.55 }, { 0.8, 0.3, 0.55 }, { 1.1, 0.25, 0.5 }, { 1.4, 0.25, 0.5 } };
        // TODO: ovdje ucitaj cijevi iz datoteke

        std::ifstream file(this->map);

        // Check if the file opened successfully
        if (!file.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return;
        }

        // Read the file line by line
        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> tokens;
            std::stringstream ss(line);
            std::string token;
            while (std::getline(ss, token, ' ')) {
                tokens.push_back(token);
            }

            pipes.push_back({ std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]) });
        }

        // Close the file
        file.close();

        // pretvori u piksele, ovisno o dimenzijama prozora
        for (Pipe& pipe : pipes) {
            pipe.x *= Parameters::WINDOW_WIDTH;
            pipe.bottomY *= Parameters::WINDOW_HEIGHT;
            pipe.topY *= Parameters::WINDOW_HEIGHT;
        }
    }

}


void Simulator::render() {}