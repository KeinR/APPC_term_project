#ifndef SIM_BODY_H_INCLUDED
#define SIM_BODY_H_INCLUDED

#include <glm/glm.hpp>

class Context;

namespace sim {
    class Simulation;
}

namespace sim {
    class Body {
        double mass; // kg
        glm::dvec3 position; // m
        glm::dvec3 velocity; // m/s
        bool t = false;
    public:
        Body();
        void setMass(double mass);
        void setPosition(const glm::dvec3 &p);
        void setVelocity(const glm::dvec3 &v);

        double getMass();
        glm::dvec3 getPosition();

        // deltaTime in seconds
        void run(Simulation &s, double deltaTime);
        virtual void render(Context &c) = 0;
    };
}

#endif
