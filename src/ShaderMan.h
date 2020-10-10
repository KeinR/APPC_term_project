#ifndef SHADER_MANAGER_H_INCLUDED
#define SHADER_MANAGER_H_INCLUDED

#include "Shader.h"

class Context;

class ShaderMan {
    unsigned int program;
    Shader body;
    Shader skybox;
    bool setShaderObj(Shader &s, Context &c);
public:
    ShaderMan(const std::string &shaderDir);
    Shader &getBody();
    Shader &getSkybox();

    void setBody(Context &c);
    void setSkybox(Context &c);
};

#endif
