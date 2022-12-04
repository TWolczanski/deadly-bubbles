#include <observer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

Observer::Observer(glm::vec3 _position, glm::vec3 _worldUp) {
    position = _position;
    worldUp = _worldUp;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    up = worldUp;
}

glm::mat4 Observer::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Observer::moveX(float speed) {
    position += speed * right;
}

void Observer::moveY(float speed) {
    position += speed * worldUp;
}

void Observer::moveZ(float speed) {
    position += speed * front;
}

void Observer::rotateX(float angle) {
    glm::mat4 rot(1.0f);
    rot = glm::rotate(rot, glm::radians(angle), right);
    glm::vec3 rotated = glm::vec3(rot * glm::vec4(front, 1.0f));
    rotated = glm::normalize(rotated);
    // prevent lookAt flip
    if (glm::angle(rotated, worldUp) >= glm::radians(10.0f) && glm::angle(rotated, -worldUp) >= glm::radians(10.0f)) {
        front = rotated;
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
}

void Observer::rotateY(float angle) {
    glm::mat4 rot(1.0f);
    rot = glm::rotate(rot, glm::radians(angle), up);
    front = glm::vec3(rot * glm::vec4(front, 1.0f));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Observer::getPosition() {
    return position;
}