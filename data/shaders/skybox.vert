#version 330 core
layout (location = 0) in vec3 pos;

out vec3 texCoords;

uniform mat4 matrix;

void main() {
    texCoords = pos;
    gl_Position = matrix * vec4(pos, 1.0);
}
