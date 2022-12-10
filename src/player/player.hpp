#ifndef PLAYER_H
#define PLAYER_H

#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <AGL3Drawable.hpp>
#include <observer.hpp>
#include <bubbles.hpp>
#include <light.hpp>

class Bubbles;

class Player : public Observer, public AGLDrawable
{
    const float radius;
    const float movSpeed;
    const float rotSpeed;
    double mouseX;
    double mouseY;
    float *vertices;
    int vertCount;
    GLuint VAO;
    GLuint VBO;

public:
    int level;

public:
    Player(float radius);
    ~Player();
    void handleInput(GLFWwindow *window, float timeDelta, Bubbles &bubbles);
    void draw(glm::mat4 view, glm::mat4 projection, PointLight pointLight, DirectionalLight directionalLight, glm::vec3 viewPos);
    friend bool collision(Player &player, Bubbles &bubbles);

private:
    void setShaders();
    void setBuffers();
};

#endif
