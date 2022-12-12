#version 330

in vec3 fragColor;
in vec3 fragPos;
in vec3 normal;

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

uniform vec3 viewPos;
uniform PointLight pointLight;
uniform PointLight playerLight;

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

void main(void) {
    color = vec4(computePointLight(pointLight) + computePointLight(playerLight), 0.5);
}
