#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

class Context;

class Matrix {
public:
    // Translation
    float x;
    float y;
    float z;
    // Scaling
    float xScale;
    float yScale;
    float zScale;

    Matrix(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    void load(const Context &c);
};

#endif
