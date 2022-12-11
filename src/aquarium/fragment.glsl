#version 330

in vec3 fragPos;
in vec3 normal;

out vec3 color;

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

vec3 computePointLight(PointLight light) {
    vec3 N = normalize(normal);
    vec3 L = normalize(light.position - fragPos);
    vec3 R = normalize(reflect(-L, N));
    vec3 V = normalize(viewPos - fragPos);
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.quadratic * (d * d) + light.linear * d + light.constant);
    vec3 materialColor = vec3(0.0, 0.0, 1.0);

    vec3 ambient = light.ambient * materialColor * attenuation;
    vec3 diffuse = light.diffuse * max(dot(N, L), 0.0) * materialColor * attenuation;
    vec3 specular = light.specular * pow(max(dot(R, V), 0.0), 32.0) * materialColor * attenuation;

    return ambient + diffuse;
}

vec3 computeDirectionalLight(DirectionalLight light) {
    vec3 N = normalize(normal);
    vec3 L = normalize(-light.direction);
    vec3 R = normalize(reflect(-L, N));
    vec3 V = normalize(viewPos - fragPos);
    vec3 materialColor = vec3(0.0, 0.0, 1.0);

    vec3 ambient = light.ambient * materialColor;
    vec3 diffuse = light.diffuse * max(dot(N, L), 0.0) * materialColor;
    vec3 specular = light.specular * pow(max(dot(R, V), 0.0), 32.0) * materialColor;

    return ambient + diffuse;
}

void main(void) {
    color = computePointLight(pointLight) + computePointLight(playerLight);
    // color = computePointLight(pointLight);
    // color = computeDirectionalLight(directionalLight);
    // color = computeDirectionalLight(directionalLight) + computePointLight(pointLight);
}
