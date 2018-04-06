#version 330 core

in vec3 normal;
out vec3 color;

//uniform sampler2D uniformTexture;

void main() {
    if (normal == vec3(0.0f, 0.0f, 1.0f))
        color = vec3(0.0f, 1.0f, 0.0f);
    else
        color = vec3(1.0f, 0.0f, 0.0f);
}
