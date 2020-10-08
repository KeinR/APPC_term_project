#include "meshgen.h"

#include <cmath>

static constexpr float PI = 3.14159;

Mesh mg::sphere(unsigned int quality) {
    constexpr float radius = 1;

    // MUST be even and > 0
    const int smoothness = quality - quality % 2;

    const int countVerticies = std::pow(smoothness + 1, 2) * 3;
    float *verticies = new float[countVerticies];
    int vi = 0;

    for (int stack = 0; stack <= smoothness; stack++) {
        float stcAngle = PI / 2 - PI * stack / smoothness;

        for (int sector = 0; sector <= smoothness; sector++) {
            float secAngle = 2 * PI * sector / smoothness;

            // x, y, z
            verticies[vi++] = radius * std::cos(stcAngle) * std::cos(secAngle);
            verticies[vi++] = radius * std::cos(stcAngle) * std::sin(secAngle);
            verticies[vi++] = radius * std::sin(stcAngle);
            // if (verticies[vi-1] == 0.0f) {
            //     std::cout << "ZERO ZERO" << std::endl;
            // }
        }
    }

    const int countIndices = (smoothness - 1) * smoothness * 3 * 2;
    unsigned int *indices = new unsigned int[countIndices];
    int ii = 0;

    for (int stack = 0; stack < smoothness; stack++) {

        int k1 = stack * (smoothness + 1);
        int k2 = k1 + smoothness + 1;

        for (int sector = 0; sector < smoothness; sector++, k1++, k2++) {
            if (stack != 0) {
                indices[ii++] = k1;
                indices[ii++] = k2;
                indices[ii++] = k1 + 1;
            }
            if (stack != smoothness - 1) {
                indices[ii++] = k2;
                indices[ii++] = k1 + 1;
                indices[ii++] = k2 + 1;
            }
        }
    }

    Mesh mesh;
    mesh.setVertices(countVerticies, verticies);
    mesh.setIndices(countIndices, indices);
    mesh.setParam(0, 3, 3, 0);

    delete[] verticies;
    delete[] indices;

    return mesh;
}
