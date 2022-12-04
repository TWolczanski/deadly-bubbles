#version 330

layout(location = 0) in vec3 vPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void) {
    gl_Position = projection * view * model * vec4(vPos, 1.0);
}