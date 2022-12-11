#include <light.hpp>
#include <constants.hpp>

void updateBubbleLights(PointLight bubbleLights[], Bubbles &bubbles) {
    for (int i = 0; i < SPECIAL_BUBBLES_COUNT; i++) {
        PointLight light = bubbleLights[i];
        int index = bubbles.special[i];
        light.position = bubbles.positions[index];
        light.quadratic = 3.0;
        light.linear = 1.5;
        light.constant = 1.0;
        if (bubbles.positions[index].y + bubbles.radii[index] >= AQUARIUM_SIZE_Y) {
            light.ambient = glm::vec3(0.0f);
            light.diffuse = glm::vec3(0.0f);
            light.specular = glm::vec3(0.0f);
        }
        else {
            light.ambient = bubbles.colors[index] * 0.2f;
            light.diffuse = bubbles.colors[index];
            light.specular = bubbles.colors[index];
        }
    }
}