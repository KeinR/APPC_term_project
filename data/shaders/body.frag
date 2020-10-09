#version 330 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D tex;

void main() {
	// FragColor = vec4(1, 0, 0, 1);
	FragColor = vec4(texture(tex, texCoords).rgb, 1);
}
