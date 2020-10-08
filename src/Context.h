#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

class ShaderMan;
class Camera;

// The mutable context
class Context {
    int matrixUniform;
    ShaderMan *shaderMan;
    Camera *camera;
public:
    Context();

    void clearUniforms();

    void setMatrixUniform(int loc);
    void setShaderMan(ShaderMan &man);
    void setCamera(Camera &cam);

    int getMatrixUniform() const;
    ShaderMan &getShaders() const;
    Camera &getCamera() const;
};

#endif
