#ifndef OBSERVER_H
#define OBSERVER_H

#include <glm/glm.hpp>

class Observer
{
protected:
    glm::vec3 position;
    glm::vec3 worldUp;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

public:
    Observer(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f));
    void moveX(float speed);
    void moveY(float speed);
    void moveZ(float speed);
    void rotateX(float angle);
    void rotateY(float angle);
    glm::mat4 getViewMatrix();
};

#endif