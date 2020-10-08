#include "Matrix.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "opengl.h"
#include "Context.h"
#include "Camera.h"

inline static float ndcX(float x, int winWidth) {
    return x / winWidth * 2 - 1;
}
inline static float ndcY(float y, int winHeight) {
    return (y / winHeight * 2 - 1) * -1;
}

Matrix::Matrix(float x, float y, float z):
    x(x), y(y), z(z), xScale(1), yScale(1), zScale(1) {
}

void Matrix::load(const Context &c) {
    glm::mat4 model(1.0f);

    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::scale(model, glm::vec3(xScale, yScale, xScale));

    model = c.getCamera().getProj() * c.getCamera().getView() * model;

    glUniformMatrix4fv(c.getMatrixUniform(), 1, GL_FALSE, glm::value_ptr(model));
}
