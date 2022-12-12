#version 330

in vec3 fragColor;
in vec3 fragPos;
in vec3 normal;
flat in int instance;

out vec4 color;

struct PointLight {
    vec3 position;

    // attenuation coefficients
    float quadratic;
    float linear;
    float constant;

    // colors of Phong components
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    vec3 direction;

    // colors of Phong components
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform PointLight pointLight;
uniform DirectionalLight directionalLight;
uniform PointLight playerLight;

#define SPECIAL_BUBBLES_COUNT 7
uniform int special[SPECIAL_BUBBLES_COUNT];
uniform PointLight bubbleLights[SPECIAL_BUBBLES_COUNT];

vec3 computePointLight(PointLight light) {
    vec3 N = normalize(normal);
    vec3 L = normalize(light.position - fragPos);
    vec3 R = normalize(reflect(-L, N));
    vec3 V = normalize(viewPos - fragPos);
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.quadratic * (d * d) + light.linear * d + light.constant);

    vec3 ambient = light.ambient * fragColor * attenuation;
    vec3 diffuse = light.diffuse * max(dot(N, L), 0.0) * fragColor * attenuation;
    vec3 specular = light.specular * pow(max(dot(R, V), 0.0), 32.0) * fragColor * attenuation;

    return ambient + diffuse + specular;
}

vec3 computeDirectionalLight(DirectionalLight light) {
    vec3 N = normalize(normal);
    vec3 L = normalize(-light.direction);
    vec3 R = normalize(reflect(-L, N));
    vec3 V = normalize(viewPos - fragPos);

    vec3 ambient = light.ambient * fragColor;
    vec3 diffuse = light.diffuse * max(dot(N, L), 0.0) * fragColor;
    vec3 specular = light.specular * pow(max(dot(R, V), 0.0), 32.0) * fragColor;

    return ambient + diffuse + specular;
}

void main(void) {
    color = vec4(computePointLight(pointLight) + computePointLight(playerLight), 0.5);
    
    // for (int i = 0; i < SPECIAL_BUBBLES_COUNT; i++) {
    //     if (special[i] < instance) {
    //         color = fragColor;
    //         return;
    //     }
    // }
    // for (int i = 0; i < SPECIAL_BUBBLES_COUNT; i++) {
    //     vec3 c = color + computePointLight(bubbleLights[i]);
    //     if (c.r < 1.0 && c.g < 1.0 && c.b < 1.0) {
    //         color = c;
    //     }
    // }
    // color = computePointLight(pointLight);
    // color = computeDirectionalLight(directionalLight);
    // color = computeDirectionalLight(directionalLight) + computePointLight(pointLight);
}
