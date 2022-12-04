#version 330

layout(location = 0) in vec3 vPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fPos;

void main(void) {
    gl_Position = model * vec4(vPos, 1.0);
    fPos = gl_Position.xyz;
    gl_Position = projection * view * gl_Position;
}