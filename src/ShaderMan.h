#ifndef SHADER_MANAGER_H_INCLUDED
#define SHADER_MANAGER_H_INCLUDED

#include "Shader.h"

class Context;

class ShaderMan {
    unsigned int program;
    Shader body;
    bool setShaderObj(Shader &s, Context &c);
public:
    ShaderMan(const std::string &shaderDir);
    Shader &getBody();

    void setBody(Context &c);
};

#endif
