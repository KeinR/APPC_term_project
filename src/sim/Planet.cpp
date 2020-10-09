#include "Planet.h"

#include <iostream>

#include "../Matrix.h"
#include "../Context.h"

sim::Planet::Planet(const std::shared_ptr<Texture> &texture): texture(texture) {
}
void sim::Planet::render(Context &c) {
    glm::vec3 pos = getPosition() * c.getScale();
    Matrix matrix(pos.x, pos.y, pos.z);
    // std::cout << "pos.x = " << pos.x << '\n';
    // std::cout << "pos.y = " << pos.y << '\n';
    // std::cout << "pos.z = " << pos.z << '\n';
    // Matrix matrix(0, 0, 0);
    matrix.load(c);
    texture->bind();
    c.renderSphere();
}
