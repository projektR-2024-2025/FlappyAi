#ifndef OUTPUT_H
#define OUTPUT_H
#include <iostream>
#include <fstream>
#include <string>

#define TYPE double

// struktura za opisivanje output noda
struct Output {
    int connection;
    TYPE value;

    friend std::ostream& operator<<(std::ostream& os, const Output& output) {
        os << output.connection << " " << output.value;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Output& output) {
        is >> output.connection >> output.value;
        return is;
    }
};

#endif // !NODE_H