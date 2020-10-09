#include "ShaderMan.h"

#include "Context.h"

bool ShaderMan::setShaderObj(Shader &s, Context &c) {
    if (s.getHandle() != program) {
        program = s.getHandle();
        s.use();
        c.clearUniforms();
        return true;
    }
    return false;
}

ShaderMan::ShaderMan(const std::string &shaderDir):
    program(0),
    body(shaderDir + "/body.vert", shaderDir + "/body.frag")
{
    body.use();
    Shader::setInt1(body.getUniform("tex"), 0);
    Shader::disuse();
}
Shader &ShaderMan::getBody() {
    return body;
}

void ShaderMan::setBody(Context &c) {
    if (setShaderObj(body, c)) {
        c.setMatrixUniform(body.getUniform("matrix"));
    }
}
