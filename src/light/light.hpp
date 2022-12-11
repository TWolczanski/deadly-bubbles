#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <bubbles.hpp>

class Bubbles;

struct PointLight
{
    glm::vec3 position;

    // attenuation coefficients
    float quadratic;
    float linear;
    float constant;

    // colors of Phong components
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct DirectionalLight
{
    glm::vec3 direction;

    // colors of Phong components
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

void updateBubbleLights(PointLight bubbleLights[], Bubbles &bubbles);

#endif