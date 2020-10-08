#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <string>
#include <map>

class Shader {
public:
    typedef int uniform_t;
    typedef unsigned int program_t;
private:
    typedef std::map<std::string, uniform_t> uniforms_t;

    program_t handle;
    uniforms_t uniforms;

    void free();
    void steal(Shader &other);
    uniform_t uni(const std::string &name);
    void createShader(const char *vertexData, unsigned int vertexLength, const char *fragmentData, unsigned int fragmentLength);
public:
    Shader(const char *vertexData, unsigned int vertexLength, const char *fragmentData, unsigned int fragmentLength);
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    Shader(Shader &&other);
    virtual ~Shader();
    Shader &operator=(Shader &&other);

    void use() const;
    static void disuse();

    uniform_t getUniform(const std::string &name);
    program_t getHandle() const;

    // ALL SETTERS REQUIRE THAT THE SHADER IS CURRENTLY BOUND!!!
    static void setMatrix4fv(uniform_t location, const float *data, unsigned int count = 1);
    static void setInt1(uniform_t location, int value);
};

#endif
