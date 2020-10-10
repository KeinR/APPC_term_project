#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

class ShaderMan;
class Camera;
class Mesh;

// The mutable context
class Context {
    int matrixUniform;
    double scale;
    ShaderMan *shaderMan;
    Camera *camera;
    Mesh *sphereMesh;
public:
    Context();

    void clearUniforms();

    void setSphereMesh(Mesh &m);

    void setMatrixUniform(int loc);
    void setScale(double s);
    void setShaderMan(ShaderMan &man);
    void setCamera(Camera &cam);

    int getMatrixUniform() const;
    double getScale() const;
    ShaderMan &getShaders() const;
    Camera &getCamera() const;

    void renderSphere() const;
};

#endif
