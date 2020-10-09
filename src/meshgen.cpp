#include "meshgen.h"

#include <cmath>

static constexpr float PI = 3.14159;

Mesh mg::sphere(unsigned int quality) {
    constexpr float radius = 1;
    constexpr int vertexSize = 8;

    // MUST be even and > 0
    const int smoothness = quality - quality % 2;

    const int countVerticies = std::pow(smoothness + 1, 2) * vertexSize;
    float *verticies = new float[countVerticies];
    int vi = 0;

    for (int stack = 0; stack <= smoothness; stack++) {
        float stcAngle = PI / 2 - PI * stack / smoothness;
        float xy = radius * std::cos(stcAngle);
        float z = radius * std::sin(stcAngle);

        for (int sector = 0; sector <= smoothness; sector++) {
            float secAngle = 2 * PI * sector / smoothness;

            // x, y, z (position)
            float x = xy * std::cos(secAngle);
            float y = xy * std::sin(secAngle);
            verticies[vi++] = x;
            verticies[vi++] = y;
            verticies[vi++] = z;
            // s, t (texture)
            verticies[vi++] = static_cast<float>(sector) / smoothness;
            verticies[vi++] = static_cast<float>(stack) / smoothness;
            // x, y, z (normals)
            // Already pointing out of center, just need to normalize
            // by magnitude
            verticies[vi++] = x / radius;
            verticies[vi++] = y / radius;
            verticies[vi++] = z / radius;
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
    mesh.setParam(0, 3, vertexSize, 0); // Position
    mesh.setParam(1, 2, vertexSize, 3); // Texture
    mesh.setParam(2, 3, vertexSize, 5); // Normal

    delete[] verticies;
    delete[] indices;

    return mesh;
}
