#version 330

layout(location = 0) in vec3 pos;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void) {
    fragPos = vec3(model * vec4(pos, 1.0));
    normal = vec3(model * vec4(pos, 0.0));
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
