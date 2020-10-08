#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

class ShaderMan;
class Camera;
class Mesh;

// The mutable context
class Context {
    int matrixUniform;
    float scale;
    ShaderMan *shaderMan;
    Camera *camera;
    Mesh *sphereMesh;
public:
    Context();

    void clearUniforms();

    void setSphereMesh(Mesh &m);

    void setMatrixUniform(int loc);
    void setScale(float s);
    void setShaderMan(ShaderMan &man);
    void setCamera(Camera &cam);

    int getMatrixUniform() const;
    float getScale() const;
    ShaderMan &getShaders() const;
    Camera &getCamera() const;

    void renderSphere() const;
};

#endif
