#ifndef CGP1NODE_H
#define CGP1NODE_H
#include <iostream>
#include <fstream>
#include <string>

#define TYPE double

namespace cgp_andrija {
    /**
     * Struktura koja opisuje gene mreze CGP jedinke.
     */
    struct CGP1Node {
        /**
         * Vrijednost koja oznacava koji se operand koristi u nodeu.
         */
        int operand;
        /**
         * Prva konekcija nodea na drugi node.
         */
        int connection1;
        /**
         * Druga konekcija nodea na drugi node.
         */
        int connection2;
        /**
         * Vrijednost koja oznacava koristi li se node.
         */
        bool used;
        /**
         * Izlazna vrijednost nakon racunanja vrijednosti.
         */
        TYPE outValue;

        /**
         * Operator overloading za pisanje gena u datoteku.
         */
        friend std::ostream& operator<<(std::ostream& os, const CGP1Node& node) {
            os << node.operand << " " << node.connection1 << " " << node.connection2 << " " << node.used << " " << 0;
            return os;
        }

        /**
         * Operator overloading za citanje gena iz datoteke.
         */
        friend std::istream& operator>>(std::istream& is, CGP1Node& node) {
            is >> node.operand >> node.connection1 >> node.connection2 >> node.used >> node.outValue;
            return is;
        }
    };
}

#endif // !NODE_H