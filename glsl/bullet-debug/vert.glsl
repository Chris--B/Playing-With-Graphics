#version 430

layout(location=0) uniform mat4x4 projection;
layout(location=1) uniform mat4x4 view;

layout(location=0) in vec3 vertex;

layout(location=10) in vec3 ambient;

out vec3 color;

void main() {
    color = ambient;
    gl_Position = projection * view * vec4(vertex, 1.0);
}
