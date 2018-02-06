#version 330 core

in vec2 UV;
out vec3 color;

uniform sampler2D uniformTexture;

void main() {
  color = texture(uniformTexture, UV).rgb;
}
