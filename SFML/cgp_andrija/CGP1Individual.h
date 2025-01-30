#ifndef CGP1INDIVIDUAL_H
#define CGP1INDIVIDUAL_H
#define TYPE double

#include <vector>
#include <sstream>
#include "CGP1Node.h"
#include "CGP1Output.h"

namespace cgp_andrija {
    /**
     * Klasa koja reprezentira jednog CGP pojedinca.
     */
    class CGP1Individual {
    private:
        std::string evalFunction(int nodeNum);
        void isUsed(int nodeNum);
        bool loopFinder(int nodeNum, std::vector<int> nodeSet);
        TYPE evalNode(int nodeNum);
        void clearInd();
    public:
        /**
         * Vector @ref CGP1Node koji reprezentira sve ulazne i gene mreze.
         */
        std::vector<CGP1Node> genes;
        /**
         * Vector @ref CGP1Output koji reprezentira sve izlazne gene.
         */
        std::vector<CGP1Output> outputGene;
        /**
         * 2D vector koji reprezentira sve aktivne grane jedinke.<br>
         * Koristi se za otklanjanje implicitnih petlji u mrezi nodeova.
         */
        std::vector<std::vector<int>> branches;
        /**
         * Broj redova u mrezi.
         */
        int rows;
        /**
         * Broj stupaca u mrezi.
         */
        int columns;
        /**
         * Broj stupaca ispred noda na koje se moze spojiti.
         */
        int levelsBack;
        /**
         * Broj ulaznih gena.
         */
        int inputs;
        /**
         * Broj izlaznih gena.
         */
        int outputs;
        /**
         * Varijabla koja oznacava je li se proslo kroz mrezu i oznacilo koji se nodeovi koriste.
         */
        int evalDone;

        /**
         * Osnovni kostruktor koji kreita praznu jedinku.
         */
        CGP1Individual();
        /**
         * Konstruktor kojim se kreira jedinka.<br>
         * Koristi se pri ucenju.
         * @param[in] genes         Vector gena.
         * @param[in] outputGene    Vector izlaznih gena.
         * @param[in] rows          Broj redova CGP mreze.
         * @param[in] columns       Broj stupaca CGP mreze.
         * @param[in] levelsBack    Broj stupaca ispred noda na koje se moze spojiti.
         * @param[in] inputs        Broj ulaznih nodova.
         * @param[in] outputs       Broj izlaznih nodova.
         */
        CGP1Individual(std::vector<CGP1Node> genes, std::vector<CGP1Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs);
        /**
         * Konstruktor kojim se kreira jedinka.<br>
         * Koristi se pri ucitavanju najbolje jedinke iz datoteke.<br>
         * Gotovo isti kao i drugi kostruktor.
         */
        CGP1Individual(std::vector<CGP1Node> genes, std::vector<CGP1Output> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs, bool evalDone);

        /**
         * Metoda za ispis svih nodova na standardni izlaz.
         */
        void printNodes();
        /**
         * Metoda za ispis funkcije koju jedinka koristi za evaluaciju skakanja.
         */
        void printFuction();
        /**
         * Metoda za izracunavanje vrijednosti u izlaznim genima za dane ulazne vrijednosti.
         * @param[in] input Vector ulazniih vrijednosti tipa double.
         */
        void evaluateValue(std::vector<TYPE> input);
        /**
         * Metoda za oznacavanje koristenih gena u mrezi.
         */
        void evaluateUsed();
        /**
         * Funkcija za izracunavanje fitness na bazi predjene udaljenosti.
         * @param[in] lenght    Predjena udaljenost.
         * @returns             Izracunati fitness.
         */
        TYPE calculateFitness(TYPE lenght);
        /**
         * Staticka metoda za ucitavanje jedinke iz datoteke.
         * @param[in] is    Istream za ulaznu datoteku.
         */
        static CGP1Individual deserialize(std::istream& is);
        /**
         * Rekurzivna funkcija za pronalazak petlji u mrezi.
         * @param[in] nodeNum  Broj trenutnog noda.
         * @param[in] nodeSet  Vector za sad prodjenih nodeova.
         * @returns             True ako je pronadjena petlja, inace false.
         */
        bool findLoops(int nodeNum, std::vector<int> nodeSet);
        /**
         * Metoda za razrjesavanje petlji u mrezi.
         */
        void resolveLoops();

        /**
         * Operator overloading za pisanje najbolje jedinke u datoteku.
         */
        friend std::ostream& operator<<(std::ostream& os, const CGP1Individual& ind) {
            os << ind.rows << " " << ind.columns << " " << ind.levelsBack << " "
                << ind.inputs << " " << ind.outputs << " " << ind.evalDone << "\n";

            os << ind.genes.size() << "\n";
            for (const auto& gene : ind.genes)
                os << gene << "\n";

            os << ind.outputGene.size() << "\n";
            for (const auto& output : ind.outputGene)
                os << output << "\n";

            return os;
        }

        /**
         * Operator overloading za citanje najbolje jedinke iz datoteke.
         */
        friend std::istream& operator>>(std::istream& is, CGP1Individual& ind) {
            is >> ind.rows >> ind.columns >> ind.levelsBack
                >> ind.inputs >> ind.outputs >> ind.evalDone;

            size_t genesSize, outputGeneSize;
            is >> genesSize;
            ind.genes.resize(genesSize);
            for (auto& gene : ind.genes)
                is >> gene;

            is >> outputGeneSize;
            ind.outputGene.resize(outputGeneSize);
            for (auto& output : ind.outputGene)
                is >> output;

            return is;
        }
    };
}

#endif