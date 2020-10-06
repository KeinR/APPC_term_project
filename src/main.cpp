
#include <iostream>
#include <exception>

#include "Program.h"

int main(int argc, char **argv) {
    int exitCode = 2;
    try {
        Program program;
        program.run();
        exitCode = 0;
    } catch (std::exception &e) {
        std::cerr << "Uncaught exception: " << e.what() << '\n';
        exitCode = 1;
    }
    return exitCode;
}
