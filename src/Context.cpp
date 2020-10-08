#include "Context.h"

#include "debug.h"

static constexpr int BAD_UNIFORM = -1;

Context::Context():
    matrixUniform(BAD_UNIFORM),
    shaderMan(nullptr),
    camera(nullptr)
{
}

void Context::clearUniforms() {
    setMatrixUniform(BAD_UNIFORM);
}

void Context::setMatrixUniform(int loc) {
    matrixUniform = loc;
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
ShaderMan &Context::getShaders() const {
    ASSERT(shaderMan != nullptr);
    return *shaderMan;
}
Camera &Context::getCamera() const {
    ASSERT(camera != nullptr);
    return *camera;
}
