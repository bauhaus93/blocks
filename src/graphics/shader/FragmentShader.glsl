#version 330 core

in vec3 uv;
in vec3 normal;
in vec4 gl_FragCoord;
out vec3 color;

struct FogData {
    float density;
    vec3 color;
};

uniform sampler2DArray textureArray;
uniform FogData fog;

void main() {

    color = texture(textureArray, uv).rgb;
    if (abs(normal.z - 1.0f) > 0.01f) {
        color *= 0.75;
    }

    float dist = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = 1.0 / exp(dist * fog.density);
    //fogFactor = 1.0 - clamp(fogFactor, 0.0, 1.0);
    color = mix(fog.color, color, fogFactor);



}
