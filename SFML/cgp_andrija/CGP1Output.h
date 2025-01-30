#ifndef CGP1OUTPUT_H
#define CGP1OUTPUT_H
#include <iostream>
#include <fstream>
#include <string>

#define TYPE double

namespace cgp_andrija {
    /**
     * Struktura koja opisuje izlazne gene CGP jedinke.
     */
    struct CGP1Output {
        /**
         * Broj koji reprezentira na koji gen je spojen izlazni gen.
         */
        int connection;
        /**
         * Izlazna vrijednost gena nakon izracuna.
         */
        TYPE value;

        /**
         * Operator overloading za pisanje izlaznog gena u datoteku.
         */
        friend std::ostream& operator<<(std::ostream& os, const CGP1Output& output) {
            os << output.connection << " " << output.value;
            return os;
        }

        /**
         * Operator overloading za citanje izlaznog gena iz datoteke.
         */
        friend std::istream& operator>>(std::istream& is, CGP1Output& output) {
            is >> output.connection >> output.value;
            return is;
        }
    };
}

#endif // !NODE_H