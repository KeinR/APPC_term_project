#include "Body.h"

#include <iostream>
#include <cmath>

#include "Simulation.h"
#include "constants.h"

// Initial mass to prevent accidental zero-division
sim::Body::Body(): mass(5), position(0.0d, 0.0d, 0.0d), velocity(0.0d, 0.0d, 0.0d) {
}
void sim::Body::setMass(double m) {
    mass = m;
}
void sim::Body::setPosition(const glm::dvec3 &p) {
    position = p;
}
void sim::Body::setVelocity(const glm::dvec3 &v) {
    velocity = v;
}

double sim::Body::getMass() {
    return mass;
}
glm::dvec3 sim::Body::getPosition() {
    return position;
}

void sim::Body::run(Simulation &s, double deltaTime) {
    // ∆-time is the ammout of time to advance the simulation;
    // the change in time, as the name suggests.
    // Time is in seconds
    glm::dvec3 f(0.0d, 0.0d, 0.0d); // force

    // Get total force acting on object
    for (Simulation::body_t &b : s) {
        // Don't loop over self
        if (b.get() != this) {

            // Formula for gravitational force:
            // F = (G * m_1 * m_2) / (r^2)
            const double r2 =
                std::pow(position.x - b->getPosition().x, 2) +
                std::pow(position.y - b->getPosition().y, 2) +
                std::pow(position.z - b->getPosition().z, 2)
            ;
            // Didn't take square root because it's squared right after

            // Prevent undefined zero-division if they collide
            if (r2 != 0) {
                // Make sure to point the force in the right direction...
                glm::dvec3 direction = glm::normalize(b->getPosition() - position);

                const double magnitude = (cst::G * b->getMass() * mass) / r2;

                f += direction * magnitude;
            }
        }
    }

    // a = F/m
    glm::dvec3 a = f / mass;

    // x = x_0 + v_0 * t + 1/2 * a * t^2
    position = position + velocity * deltaTime + a / 2.0d * static_cast<double>(std::pow(deltaTime, 2));

    // v = v_0 + a * t
    velocity = velocity + a * deltaTime;
}
