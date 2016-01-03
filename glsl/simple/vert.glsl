#version 430

layout(location=0) uniform mat4x4 projection;
layout(location=1) uniform mat4x4 view;
layout(location=2) uniform mat4x4 model;
layout(location=3) uniform vec3   sunlight;

layout(location=0) in vec3 vertex;
layout(location=1) in vec3 normal;

out vec3 color;

void main() {
    vec3 ambient = vec3(0.2);

    // TODO: Transform the normal.
    vec3 n = normalize(normal);
    vec3 material = vec3(0.5);

    // Diffuse lighting.
    float d = max(dot(sunlight, n), 0);

    color = d * material + ambient * material;
    gl_Position = projection * view * model * vec4(vertex, 1.0);
}
