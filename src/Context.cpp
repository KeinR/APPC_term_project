#include "Context.h"

#include "debug.h"
#include "Mesh.h"

static constexpr int BAD_UNIFORM = -1;

Context::Context():
    matrixUniform(BAD_UNIFORM),
    scale(1.0f),
    shaderMan(nullptr),
    camera(nullptr),
    sphereMesh(nullptr)
{
}

void Context::clearUniforms() {
    setMatrixUniform(BAD_UNIFORM);
}

void Context::setSphereMesh(Mesh &m) {
    sphereMesh = &m;
}

void Context::setMatrixUniform(int loc) {
    matrixUniform = loc;
}
void Context::setScale(double s) {
    scale = s;
}
void Context::setShaderMan(ShaderMan &man) {
    shaderMan = &man;
}
void Context::setCamera(Camera &cam) {
    camera = &cam;
}

int Context::getMatrixUniform() const {
    ASSERT(matrixUniform != BAD_UNIFORM);
    return matrixUniform;
}
double Context::getScale() const {
    // Not undefined, but still very bad
    ASSERT(scale > 0.0d);
    return scale;
}
ShaderMan &Context::getShaders() const {
    ASSERT(shaderMan != nullptr);
    return *shaderMan;
}
Camera &Context::getCamera() const {
    ASSERT(camera != nullptr);
    return *camera;
}

void Context::renderSphere() const {
    ASSERT(sphereMesh != nullptr);
    sphereMesh->render();
}
