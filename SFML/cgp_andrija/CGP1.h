#ifndef CGP1_H
#define CGP1_H
#define TYPE double

#include <vector>
#include <string>
#include "CGP1Individual.h"
#include "CGP1Node.h"
#include "../SelectionScreen.h"
#include "../Controller.h"

namespace cgp_andrija {
    const int MAX_MAP_SIZE = 5000;
    const int NUM_OPERANDS = 9;
    const int VIEW_DISTANCE = 13;

    const int GENERATIONS = 5000;
    const int ROWS = 20;
    const int COLUMNS = 20;
    const int LEVELS_BACK = 3;
    const int INPUTS = 6;
    const int OUTPUTS = 1;
    const int MUTATIONS = 6;
    /**
     * Broj jedinki u generaciji.
     */
    const int POPULATION = 20;
    /**
     * Naziv datoteke koja sadrzi najbolju jedinku.
     */
    const std::string bestFile = "CGP_best.txt";

    /**
     * Klasa koja opisuje CGP instancu.
     */
    class CGP1 {
    private:
        int generations, rows, columns, levelsBack, inputs, outputs, mutations;
        sf::RenderWindow& window;
    public:
        /**
         * Konstruktor za CGP1 klasu.
         * @param[in] generations   Broj generacija koji ce se izvrtiti pri ucenju.
         * @param[in] rows          Broj redova CGP mreze.
         * @param[in] columns       Broj stupaca CGP mreze.
         * @param[in] levelsBack    Broj stupaca ispred noda na koje se moze spojiti.
         * @param[in] inputs        Broj ulaznih nodova.
         * @param[in] outputs       Broj izlaznih nodova.
         * @param[in] mutations     Broj mutacija genoma po jedinki.
         * @param[in] window        Trenutno otvoreni SFML prozor.
         */
        CGP1(int generations, int rows, int columns, int levelsBack, int inputs, int outputs, int mutations, sf::RenderWindow& window)
            : generations(generations), rows(rows), columns(columns), levelsBack(levelsBack), inputs(inputs), outputs(outputs), mutations(mutations), window(window) {
        };

        /**
         * Funkcija za generiranje inicijalne populacije.<br>
         * Broj jedinki u populaciji ovisi o konstanti POPULATION.<br>
         * Ostali parametri su navedeni u konstruktoru.
         */
        std::vector<CGP1Individual> generatePopulation(int rows, int columns, int levelsBack, int inputs, int outputs);

        /**
         * Funkcija za kreiranje nove generacije populacije na bazi roditeljske jedinke.<br>
         * Koristi se <b>Point Mutacija</b> kojom se u roditeljskoj jedinci mutira dani
         * broj gena kako bi se kreirala nova jedinka.<br>
         * @param[in] numMut    Broj mutacija gena po jedinki.
         * @param[in] parent    Najbolja jedinka iz prosle generacija, roditelj za novu.
         */
        std::vector<CGP1Individual> mutate(int numMut, CGP1Individual parent);

        /**
         * Funkcija za kreiranje nove generacije populacije na bazi roditeljske jedinke.<br>
         * Koristi se <b>Goldman Mutacija</b> kojom se u roditeljskoj jedinci mutiraju geni
         * sve dok se ne dode do gena koji se aktivno koristi.
         * Taj gen se jos promjeni i s njime zavrsava mutacija nove jedinke.
         * @param[in] parent    Najbolja jedinka iz prosle generacija, roditelj za novu.
         */
        std::vector<CGP1Individual> mutate(CGP1Individual parent);

        /**
         * Funkcija za pokretanje ucenja CGPa s parametrima danim u konstruktoru i konstantama.
         */
        CGP1Individual runCGP();

        /**
         * Funkcija za pokretanje CGPa, ovisno o parametru @ref Parameters::action ovisi sto tocno radi.
         * @param[in] window    Trenutno otvoreni SFML prozor.
         */
        static CGP1Controller* CGPMain(sf::RenderWindow& window);
    };
}

#endif
