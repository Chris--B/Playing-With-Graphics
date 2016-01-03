#version 430

layout(location=0) uniform mat4x4 projection;
layout(location=1) uniform mat4x4 view;
layout(location=2) uniform mat4x4 model;

layout(location=0) in vec3 vertex;

out vec3 color;

void main() {
    color = vec3(gl_VertexID+1.0) / 1000000.0;
    gl_Position = projection * view * model * vec4(vertex, 1.0);
}
