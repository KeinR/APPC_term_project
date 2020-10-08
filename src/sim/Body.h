#ifndef SIM_BODY_H_INCLUDED
#define SIM_BODY_H_INCLUDED

#include <glm/glm.hpp>

class Context;

namespace sim {
    class Simulation;
}

namespace sim {
    class Body {
        float mass; // kg
        glm::vec3 position; // m
        glm::vec3 velocity; // m/s
        bool t = false;
    public:
        Body();
        void setMass(float mass);
        void setPosition(const glm::vec3 &p);
        void setVelocity(const glm::vec3 &v);

        float getMass();
        glm::vec3 getPosition();

        void run(Simulation &s, float deltaTime);
        virtual void render(Context &c) = 0;
    };
}

#endif
