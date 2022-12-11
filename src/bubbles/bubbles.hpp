#ifndef BUBBLES_H
#define BUBBLES_H

#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include <AGL3Drawable.hpp>
#include <player.hpp>
#include <light.hpp>
#include <constants.hpp>

class Player;
struct PointLight;
struct DirectionalLight;

class Bubbles : public AGLDrawable
{
    const int count;
    const float speed;
    const float growthRate;
    const float maxRadius;

    glm::vec3 *positions;
    float *radii;
    glm::vec3 *colors;
    int special[SPECIAL_BUBBLES_COUNT];

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
    void draw(glm::mat4 view, glm::mat4 projection, PointLight pointLight, DirectionalLight directionalLight, PointLight playerLight, PointLight bubbleLights[], glm::vec3 viewPos, double time, double timeDelta);
    friend bool collision(Player &player, Bubbles &bubbles);
    friend void updateBubbleLights(PointLight bubbleLights[], Bubbles &bubbles);

private:
    void setShaders();
    void setBuffers();
};

#endif