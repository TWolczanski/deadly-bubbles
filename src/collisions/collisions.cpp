#include <collisions.hpp>
#include <glm/glm.hpp>

bool collision(Player &player, Bubbles &bubbles) {
    for (int i = 0; i < bubbles.count; i++) {
        if (glm::distance(player.position, bubbles.positions[i]) <= player.radius + bubbles.radii[i]) {
            return true;
        }
    }
    return false;
}