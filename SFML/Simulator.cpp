#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "parameters.h"


Simulator::Simulator() : running(true)
{}


bool Simulator::initialize(Bird& bird) 
{
    srand(static_cast<unsigned>(time(0)));

    // idemo li dalje
    running = true;

    // ptica, cijevi
    std::vector<Pipe> pipes;
    initializeMap(bird);

    // stvori GUI prozor samo ako treba
    sf::RenderWindow* pw = NULL;
    if (!simulationOnly)
        pw = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Flappy AI");
    window = pw;

    // izgled ptice
    // TODO: stavite neki sprite
    birdShape.setSize(sf::Vector2f(BIRD_SIZE, BIRD_SIZE));
    birdShape.setFillColor(sf::Color::Yellow);

    // izgled cijevi
    // TODO: sprite
    topPipe.setSize(sf::Vector2f(PIPE_WIDTH, WINDOW_HEIGHT));
    topPipe.setFillColor(sf::Color::Green);
    bottomPipe.setSize(sf::Vector2f(PIPE_WIDTH, WINDOW_HEIGHT));
    bottomPipe.setFillColor(sf::Color::Green);

    // tlo na 50 piksela od dna prozora
    ground.setSize(sf::Vector2f(WINDOW_WIDTH, 50));
    ground.setFillColor(sf::Color::Green);
    ground.setPosition(0, WINDOW_HEIGHT - 50);

    clock.restart();

    return true;
}


bool Simulator::isRunning() const 
{
    if (simulationOnly)
        return running;
    return true;
}


bool Simulator::simulateFrame(Bird& bird, float deltaTime) 
{
    // brzina i visina
    bird.velocity += GRAVITY * deltaTime;
    bird.position += bird.velocity * deltaTime;

    // pomakni cijevi, provjeri sudar
    for (auto& pipe : pipes) {
        pipe.x -= SPEED * deltaTime;

        // je li sudar
        if (pipe.x < (BIRD_OFFSET + BIRD_SIZE) && (pipe.x + PIPE_WIDTH) > BIRD_OFFSET) { // presjek po sirini
            if (bird.position < pipe.topY || bird.position + BIRD_SIZE > pipe.bottomY) { // presjek po visini
                return false; // Collision detected
            }
        }

        if (randomPipes)
            // stvori slucajnu novu cijev kad ovu predjemo
            if (pipe.x + PIPE_WIDTH < 0) {
                float pipeHeight = (float) (rand() % 200 + 100);
                pipe.x = WINDOW_WIDTH;
                pipe.topY = pipeHeight;
                pipe.bottomY = pipeHeight + PIPE_GAP;
            }
    }

    // je li sudar s tlom ili vrhom prozora
    if (bird.position < 0 || bird.position + BIRD_SIZE > WINDOW_HEIGHT - 50) {
        return false;
    }

    // koliko smo prosli
    bird.distance += SPEED * deltaTime;

    return true; // nema sudara
}


void Simulator::update(Bird& bird) 
{
    if (simulationOnly || window->isOpen()) {
        if (!simulationOnly) {
            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window->close();
                }
                // space za skok
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                    bird.velocity = JUMP_SPEED;
                }
                // 'r' za restart
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    initializeMap(bird);
                    running = true;
                }
            }
        }

        // najvaznija varijabla: period simulacije
        float deltaTime;
        if (simulationOnly)
            deltaTime = 0.03f;  // uzmimo kao da su prosle 3 stotinke (simulirano vrijeme)
        else
            deltaTime = clock.restart().asSeconds();    // koliko je proslo ako koristimo prikaz (real time)

        // korak simulacije
        if (running && !simulateFrame(bird, deltaTime)) {
            std::cout << "Prijedjena udaljenost: " << bird.distance << "\n";
            running = false;
            //resetGame(bird, pipes, distance);
            if (simulationOnly) {
                return; // nema reseta ako nema prikaza
            }
        }

        // ako nije simulacija, ovdje se sve crta
        if (!simulationOnly) {
            window->clear(sf::Color(135, 206, 250)); // nebo
            window->draw(ground);

            // crtaj pticu
            birdShape.setPosition(BIRD_OFFSET, bird.position);
            window->draw(birdShape);

            // crtaj cijevi - samo prve 4
            for (int i = 0; i < pipes.size(); i++) {
                topPipe.setPosition(pipes[i].x, 0);
                topPipe.setSize(sf::Vector2f(PIPE_WIDTH, pipes[i].topY));
                bottomPipe.setPosition(pipes[i].x, pipes[i].bottomY);
                bottomPipe.setSize(sf::Vector2f(PIPE_WIDTH, WINDOW_HEIGHT - pipes[i].bottomY));
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
    bird.position = WINDOW_HEIGHT / 2;
    bird.velocity = 0.0;
    bird.distance = 0.0;

    // nove cijevi
    pipes.clear();

    // a) slucajne cijevi
    if (randomPipes)
        for (float x = WINDOW_WIDTH / 2; x < 1.5 * WINDOW_WIDTH; x += 300) {
            float pipeHeight = (float) (rand() % 200 + 100);
            pipes.push_back({ x, pipeHeight, pipeHeight + PIPE_GAP });
        }
    // b) hard coded ili ucitane cijevi
    else {
        // cijevi su dimenzionirane relativno na velicinu prozora, [0, 1]
        //pipes = std::vector<Pipe>{ { 0.5, 0.3, 0.55 }, { 0.8, 0.3, 0.55 }, { 1.1, 0.25, 0.5 }, { 1.4, 0.25, 0.5 } };
        // TODO: ovdje ucitaj cijevi iz datoteke

        //std::ifstream file("Map.txt");
        std::ifstream file("/Users/nikson/Documents/GitHub/FlappyAi/SFML/Map.txt");

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
            pipe.x *= WINDOW_WIDTH;
            pipe.bottomY *= WINDOW_HEIGHT;
            pipe.topY *= WINDOW_HEIGHT;
        }
    }

}


void Simulator::render() {}