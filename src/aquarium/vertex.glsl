#version 330

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normal;

void main(void) {
    gl_Position = model * vec4(Pos, 1.0);
    fragPos = gl_Position.xyz;
    gl_Position = projection * view * gl_Position;
    normal = vec3(model * vec4(Normal, 0.0));
}
