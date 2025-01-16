#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <fstream>
#include <string>

#define TYPE double

// struktura za opisivanje noda
struct Node {
    int operand;
    int connection1;
    int connection2;
    bool used;
    TYPE outValue;

    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        os << node.operand << " " << node.connection1 << " " << node.connection2 << " " << node.used << " " << 0;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Node& node) {
        is >> node.operand >> node.connection1 >> node.connection2 >> node.used >> node.outValue;
        return is;
    }
};

#endif // !NODE_H