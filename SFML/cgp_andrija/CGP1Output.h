#ifndef CGP1OUTPUT_H
#define CGP1OUTPUT_H
#include <iostream>
#include <fstream>
#include <string>

#define TYPE double

// struktura za opisivanje output noda
struct CGP1Output {
    int connection;
    TYPE value;

    friend std::ostream& operator<<(std::ostream& os, const CGP1Output& output) {
        os << output.connection << " " << output.value;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, CGP1Output& output) {
        is >> output.connection >> output.value;
        return is;
    }
};

#endif // !NODE_H