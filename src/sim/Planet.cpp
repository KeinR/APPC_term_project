#include "Planet.h"

#include <iostream>

#include "../Matrix.h"
#include "../Context.h"

sim::Planet::Planet(const std::shared_ptr<Texture> &texture, float scale):
    texture(texture), scale(scale) {
}
void sim::Planet::render(Context &c) {
    glm::vec3 pos = getPosition() * c.getScale();
    Matrix matrix(pos.x, pos.y, pos.z);
    matrix.xScale = scale;
    matrix.yScale = scale;
    matrix.zScale = scale;
    // std::cout << "pos.x = " << pos.x << '\n';
    // std::cout << "pos.y = " << pos.y << '\n';
    // std::cout << "pos.z = " << pos.z << '\n';
    // Matrix matrix(0, 0, 0);
    matrix.load(c);
    texture->bind();
    c.renderSphere();
}
