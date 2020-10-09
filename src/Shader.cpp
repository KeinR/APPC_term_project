#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <array>

#include "opengl.h"
#include "debug.h"
#include "error.h"

static constexpr unsigned int LOG_BUFFER_SIZE = 512;

/**
* Compiles the given shader and returns a handle to the OpenGL object.
* @param [in] type The type (GL_VERTEX_SHADER/GL_FRAGMENT_SHADER)
* @param [in] data Pointer to shader data
* @param [in] length Length of `data`
* @param [out] error String to dump errors. Untouched if no errors occurred.
* @return ID of the newly created shader object, or 0 on error [<a href="http://docs.gl/es3/glCreateShader">ref</a>]
*/
static GLuint compileShader(GLenum type, const char *data, int length);

/**
* Links two shaders (vertex and fragment) and returns the resulting
* shader program.
* @param [in] vertObject The vertex object
* @param [in] fragObject The fragment object
* @param [out] error String to dump errors. Untouched if no errors occurred.
* @return ID of the resulting shader program, or [undefined] on error (I don't see it specified anywhere in the docs(?))
*/
static GLuint linkShaders(GLuint vertObject, GLuint fragObject);

static char *loadFile(const std::string &path, int &length);

static void delShaders(GLuint vert, GLuint frag);

static const char *getShaderTypeName(GLenum type);

Shader::Shader(const char *vertexData, unsigned int vertexLength, const char *fragmentData, unsigned int fragmentLength) {
    createShader(vertexData, vertexLength, fragmentData, fragmentLength);
}
Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    int vertLen, fragLen;
    char *vertData = nullptr;
    char *fragData = nullptr;

    try {
        vertData = loadFile(vertexPath, vertLen);
        fragData = loadFile(fragmentPath, fragLen);
        createShader(vertData, vertLen, fragData, fragLen);
    } catch (std::exception &e) {
        if (vertData != nullptr) {
            delete[] vertData;
        }
        if (fragData != nullptr) {
            delete[] fragData;
        }
        serror() << "Failed to load shader resource files: " << e.what();
    }
}
Shader::~Shader() {
    free();
}

void Shader::createShader(const char *vertexData, unsigned int vertexLength, const char *fragmentData, unsigned int fragmentLength) {
    #define CLEAN delShaders(vertShader, fragShader)

    GLuint vertShader = 0;
    GLuint fragShader = 0;
    handle = 0;
    try {
        vertShader = compileShader(GL_VERTEX_SHADER, vertexData, vertexLength);
        fragShader = compileShader(GL_FRAGMENT_SHADER, fragmentData, fragmentLength);
        try {
            handle = linkShaders(vertShader, fragShader);
            CLEAN;
        } catch (std::exception &e) {
            CLEAN;
            serror() << "Failed to link shaders: " << e.what();
        }
    } catch (std::exception &e) {
        CLEAN;
        serror() << "Failed to compile shaders: " << e.what();
    }

    #undef CLEAN
}

void Shader::steal(Shader &other) {
    handle = other.handle;
    other.handle = 0;
}

Shader::uniform_t Shader::uni(const std::string &name) {
    uniforms_t::iterator loc = uniforms.find(name);
    uniform_t location;
    if (loc == uniforms.end()) {
        // Cache miss
        GLenum error;
        error = glGetError();
        if (error != GL_NO_ERROR) {
            serror() << "GL err before getting uniform location: " << serror::glErrStr(error);
        }
        location = glGetUniformLocation(handle, name.c_str());
        uniforms[name] = location;
        error = glGetError();
        if (error != GL_NO_ERROR) {
            serror() << "GL err after call to glGetUniformLocation(this->handle,name): " << serror::glErrStr(error);
        }
    } else {
        location = loc->second;
    }
    return location;
}

void Shader::free() {
    if (handle != 0) {
        glDeleteProgram(handle);
    }
}

void Shader::use() const {
    glUseProgram(handle);
}

Shader::uniform_t Shader::getUniform(const std::string &name) {
    return uni(name);
}

void Shader::setMatrix4fv(uniform_t location, const float *data, unsigned int count) {
    glUniformMatrix4fv(location, count, GL_FALSE, data);
}

void Shader::setInt1(uniform_t location, int value) {
    glUniform1i(location, value);
}

void Shader::disuse() {
    glUseProgram(0);
}

Shader::program_t Shader::getHandle() const {
    return handle;
}

GLuint compileShader(GLenum type, const char *data, int length) {
    GLuint shader;
    shader = glCreateShader(type);
    // Load shader source code
    glShaderSource(shader, 1, &data, &length);
    // Compile code
    glCompileShader(shader);
    // Check for failure
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::array<char, LOG_BUFFER_SIZE> log;
        glGetShaderInfoLog(shader, log.size(), NULL, log.data());
        glDeleteShader(shader);
        std::string msg;
        msg += "[";
        msg += getShaderTypeName(type);
        msg += " shader]: \n";
        msg += log.data();
        throw std::runtime_error(msg);
    }
    return shader;
}

GLuint linkShaders(GLuint vertObject, GLuint fragObject) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertObject);
    glAttachShader(shaderProgram, fragObject);
    glLinkProgram(shaderProgram);
    // Check for failure
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        std::array<char, LOG_BUFFER_SIZE> log;
        glGetProgramInfoLog(shaderProgram, log.size(), NULL, log.data());
        glDeleteProgram(shaderProgram);
        throw std::runtime_error("\n " + std::string(log.data()));
    }
    return shaderProgram;
}

char *loadFile(const std::string &path, int &length) {
    std::ifstream file;
    // Ensure exception safety
    file.exceptions(std::ios::goodbit);

    file.open(path);
    if (!file.good()) {
        throw std::invalid_argument("Cannot open file \"" + path + "\"");
    }

    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);

    char *data = new char[length];

    file.read(data, length);

    if (!file.good()) {
        delete[] data;
        throw std::invalid_argument("Failed to read from file \"" + path + "\"");
    }

    return data;
}

void delShaders(GLuint vert, GLuint frag) {
    glDeleteShader(vert);
    glDeleteShader(frag);
}

const char *getShaderTypeName(GLenum type) {
    switch (type) {
        case GL_VERTEX_SHADER: return "vertex";
        case GL_FRAGMENT_SHADER: return "fragment";
        default: return "unknown";
    }
}
