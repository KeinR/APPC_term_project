#ifndef SIM_SIMULATION_H_INCLUDED
#define SIM_SIMULATION_H_INCLUDED

#include <vector>
#include <memory>

#include "Body.h"

class Context;

namespace sim {
    class Simulation {
    public:
        typedef std::shared_ptr<Body> body_t;
        typedef std::vector<body_t> data_t;
        typedef data_t::iterator iterator;
    private:
        data_t data;
    public:
        Simulation();
        void push(const body_t &b);
        iterator begin();
        iterator end();
        void run(float deltaTime);
        void render(Context &c);
    };
}

#endif
