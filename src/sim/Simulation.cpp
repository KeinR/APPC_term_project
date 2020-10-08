#include "Simulation.h"

sim::Simulation::Simulation() {
}
void sim::Simulation::push(const body_t &b) {
    data.push_back(b);
}
sim::Simulation::iterator sim::Simulation::begin() {
    return data.begin();
}
sim::Simulation::iterator sim::Simulation::end() {
    return data.end();
}
void sim::Simulation::run(float deltaTime) {
    for (body_t &b : data) {
        b->run(*this, deltaTime);
    }
}
void sim::Simulation::render(Context &c) {
    for (body_t &b : data) {
        b->render(c);
    }
}
