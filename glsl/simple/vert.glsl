#version 430

layout(location=0) uniform mat4x4 MvP;

layout(location=0) in vec3 vertex;

void main() {
    gl_Position = MvP * vec4(vertex, 1.0);
}
