#include "error.h"

#include <stdexcept>
#include <iostream>

serror::serror(bool severe): severe(severe) {
}
serror::~serror() {
    doRaise();
}
void serror::doRaise() {
    raise(str(), severe);
}

void serror::raise(const std::string &message, bool severe) {
    if (severe) {
        throw std::runtime_error("SEVERE: " + message);
    } else {
        std::cerr << "Error: " << message << '\n';
    }
}
