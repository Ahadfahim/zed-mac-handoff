// main.cpp -- drives the leak from today's lecture slip, Q1.
#include "Buffer.hpp"

int main() {
    Buffer b(4);
    b.grow(8);   // triggers the leak -- see Valgrind's "definitely lost" report
    return 0;
}
