#ifndef CGP1NODE_H
#define CGP1NODE_H
#include <iostream>
#include <fstream>
#include <string>

#define TYPE double

// struktura za opisivanje noda
struct CGP1Node {
    int operand;
    int connection1;
    int connection2;
    bool used;
    TYPE outValue;

    friend std::ostream& operator<<(std::ostream& os, const CGP1Node& node) {
        os << node.operand << " " << node.connection1 << " " << node.connection2 << " " << node.used << " " << 0;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, CGP1Node& node) {
        is >> node.operand >> node.connection1 >> node.connection2 >> node.used >> node.outValue;
        return is;
    }
};

#endif // !NODE_H