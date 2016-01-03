#version 430

layout(location=0) uniform mat4x4 projection;
layout(location=1) uniform mat4x4 view;
layout(location=2) uniform mat4x4 model;
layout(location=3) uniform vec3   sunlight;

layout(location=0) in vec3 vertex;
layout(location=1) in vec3 normal;

layout(location=10) in vec3 ambient;
layout(location=11) in vec3 diffuse;

out vec3 color;

void main() {
    vec3 ambient = 0.2 * diffuse;

    // TODO: Transform the normal.
    vec3 n = normalize(normal);
    vec3 l = normalize(sunlight);

    // Diffuse lighting.
    float d = 0.5 * max(dot(l, n), 0);

    color = d * diffuse + ambient * diffuse;
    gl_Position = projection * view * model * vec4(vertex, 1.0);
}
