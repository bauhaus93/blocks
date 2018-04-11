#version 330 core

in vec3 uv;
in vec3 normal;
out vec3 color;

uniform sampler2DArray textureArray;

void main() {
    color = texture(textureArray, uv).rgb;
    if (abs(normal.z - 1.0f) > 0.01f) {
        color *= 0.75;
    }

}
