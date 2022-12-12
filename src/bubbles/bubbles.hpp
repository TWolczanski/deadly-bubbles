#ifndef BUBBLES_H
#define BUBBLES_H

#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include <AGL3Drawable.hpp>
#include <player.hpp>
#include <light.hpp>
#include <constants.hpp>

class Player;

class Bubbles : public AGLDrawable
{
    const int count;
    const float speed;
    const float growthRate;
    const float maxRadius;

    glm::vec3 *positions;
    float *radii;
    glm::vec3 *colors;

    glm::vec3 *colorBuffer;
    glm::mat4 *modelBuffer;
    float *vertices;
    int vertCount;
    GLuint VAO;
    GLuint sphereVBO;
    GLuint colorVBO;
    GLuint modelVBO;

public:
    Bubbles(int count, float speed, float growthRate);
    ~Bubbles();
    void draw(glm::mat4 view, glm::mat4 projection, PointLight pointLight, PointLight playerLight, glm::vec3 viewPos, double time, double timeDelta);
    friend bool collision(Player &player, Bubbles &bubbles);

private:
    void setShaders();
    void setBuffers();
};

#endif