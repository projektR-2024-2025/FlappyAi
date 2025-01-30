#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include "Parameters.h"

/**
 * Struktura koja opisuje jednu cijev u igri.
 */
struct Pipe {
    /**
     * Udaljenost lijevog brida cijevi od lijevog ruba ekrana.
     */
    float x;
    /**
     * Visina gornjeg dijela cijevi.
     */
    float topY;
    /**
     * Visina donjeg dijela cijevi.
     */
    float bottomY;
};

/**
 * Klasa koja predstavlja simulator igrice.
 */
class Simulator {
public:
    /**
     * Opci konstruktor za simulator.
     */
    Simulator();
    /**
     * Konstruktor koji prima vec otvoreni SFML prozor.
     * @param[in] window    Otvoreni SFML prozor.
     */
    Simulator(sf::RenderWindow* window);
    /**
     * Konstruktor koji prima mapu na kojoj ce se vrtiti.
     * @param[in] map   Mapa koja ce se vrtit.
     */
    Simulator(std::string map);
    /**
     * @deprecated Vise se ne koristi.
     * Metoda koristena u starom simulatoru.
     */
    void initializeMap(std::string map);
    /**
     * @deprecated Vise se ne koristi.
     * Metoda koristena u starom simulatoru.
     */
    void loadNextColumn(int pos = viewWidth - 1);

    /**
     * Funkcija za kreiranje i prikazivanje prvobitnog stanja ekrana.<br>
     * Takoder i za zapocinjanje igre.
     * @param[in] agent Objekt pricice koja se koristi u igri.
     * @returns         True ako je sve uspjesno izvedeno.
     */
    bool initialize(Bird& agent);
    /**
     * Metoda koja postavlja prvobitno stanje mape.<br>
     * Ako je Parameters::randomPipes true onda se generiraju prva 4 stupa.<br>
     * U protivnom se ucita mapa iz datoteke dana kod kreiranja simulatora<br>
     * (ako nije navedena, onda prva mapa iz Parameters::maps).
     * @param[in] agent Objekt pricice koja se koristi u igri.
     */
    void initializeMap(Bird& agent);
    /**
     * Funkcija za simuliranje novog okvira. Racuna se nova pozicija svih objekata i pomice se ekran.<br>
     * Ako je stup predjen onda i Parameters::randomPipes je true, generira se novi stup.
     * @param[in] deltaTime Vrijeme proteklo od predhodnog okvira.
     * @param[in] agent     Objekt pricice koja se koristi u igri.
     * @returns             True ako nema sudara, inace false.
     */
    bool simulateFrame(Bird& bird, float deltaTime);

    /**
     * Funkcija koja vraca je li simulator pokrenut.
     * @returns Ako je Parameters::simulationOnly true ili ako je Parameters::action TRAIN i Parameters::simulationOnly false<br>
     *          vrijednost varijable @code running @endcode, inace true.
     */
    bool isRunning() const;
    /**
     * @deprecated Vise se ne koristi.
     * Metoda koristena u starom simulatoru.
     */
    bool isScoreScreenActive() const;
    /**
     * @deprecated Vise se ne koristi.
     * Metoda koristena u starom simulatoru.
     */
    bool handleEvents(Bird& agent);
    /**
     * @deprecated Vise se ne koristi.
     * Metoda koristena u starom simulatoru.
     */
    void handleScoreEvents();
    /**
     * Metoda koja azurira jedan trenutak simulacije.
     * @param[in] agent     Objekt pricice koja se koristi u igri.
     */
    void update(Bird& agent);
    /**
     * @deprecated Vise se ne koristi.
     * Metoda koristena u starom simulatoru.
     */
    void render();

    /**
     * @deprecated Vise se ne koristi.
     * Varijabla koristena u starom simulatoru.
     */
    static constexpr int groundLevel = 20;
    /**
     * @deprecated Vise se ne koristi.
     * Varijabla koristena u starom simulatoru.
     */
    static constexpr int viewWidth = 35;
    /**
     * @deprecated Vise se ne koristi.
     * Varijabla koristena u starom simulatoru.
     */
    bool obstacleMap[groundLevel][viewWidth];

    /**
     * Vektor cijevi koji predstavlja mapu.
     */
    std::vector<Pipe> pipes;
    /**
     * SFML prozor u kojem se sve prikazuje.
     */
    sf::RenderWindow* window;
    /**
     * SFML sat kojim se odreduje deltaTime ako je Parameters::variableFPS true.
     */
    sf::Clock clock;
    /**
     * SFML oblik kojim se odreduje pticica.
     */
    sf::RectangleShape birdShape;
    /**
     * SFML oblik kojim se odreduje gornji dio cijevi.
     */
    sf::RectangleShape topPipe;
    /**
     * SFML oblik kojim se odreduje donji dio cijevi.
     */
    sf::RectangleShape bottomPipe;
    /**
     * SFML oblik kojim se odreduje tlo.
     */
    sf::RectangleShape ground;

private:
    std::vector<int> mapEmpty;
    std::vector<int> mapCeiling;
    std::vector<int> mapGround;
    int mapReadIndex = 0;
    bool pillarFlag = 0;
    std::string map = Parameters::maps.at(0);

    bool running;
    bool scoreScreen;
    int birdPosition;
};

#endif // SIMULATOR_H
