#version 330 core

in vec2 uv;
in vec3 normal;
out vec3 color;

uniform sampler2D uniformTexture;

void main() {
    color = texture(uniformTexture, uv).rgb;
    if (abs(normal.z - 1.0f) > 0.01f) {
        color *= 0.75;
    }
}
