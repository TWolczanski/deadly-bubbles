#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in mat4 model;

out vec3 fragColor;
out vec3 fragPos;
out vec3 normal;
flat out int instance;

uniform mat4 view;
uniform mat4 projection;

void main(void) {
    fragPos = vec3(model * vec4(pos, 1.0));
    normal = vec3(model * vec4(pos, 0.0));
    fragColor = color;
    instance = gl_InstanceID;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
