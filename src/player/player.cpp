#include <player.hpp>
#include <sphere.hpp>
#include <constants.hpp>
#include <collisions.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Player::Player() : AGLDrawable(), radius(PLAYER_RADIUS), Observer(glm::vec3(AQUARIUM_SIZE_X / 2, AQUARIUM_SIZE_Y / 2, PLAYER_RADIUS)), movSpeed(0.15), rotSpeed(30.0), mouseX(-1), mouseY(-1), level(1)
{
    rotateY(180.0);
    setShaders();
    setBuffers();
}

Player::~Player()
{
    delete[] vertices;
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Player::handleInput(GLFWwindow *window, float timeDelta, Bubbles &bubbles)
{
    // movement
    glm::vec3 oldPos = position;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        moveX(-movSpeed * timeDelta);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        moveX(movSpeed * timeDelta);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        moveY(movSpeed * timeDelta);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        moveY(-movSpeed * timeDelta);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    {
        moveZ(movSpeed * timeDelta);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    {
        moveZ(-movSpeed * timeDelta);
    }
    if (position != oldPos) {
        if (position.x < radius || position.x > AQUARIUM_SIZE_X - radius ||
            position.y < radius || position.y > AQUARIUM_SIZE_Y - radius ||
            position.z < radius
        ) {
            position = oldPos;
        }
        else if (collision(*this, bubbles)) {
            level = 0;
        }
        else if (position.z >= AQUARIUM_SIZE_Z - radius) {
            level++;
            position = glm::vec3(AQUARIUM_SIZE_X / 2, AQUARIUM_SIZE_Y / 2, radius);
        }
    }

    // rotation
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        rotateX(rotSpeed * timeDelta);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        rotateX(-rotSpeed * timeDelta);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        rotateY(rotSpeed * timeDelta);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        rotateY(-rotSpeed * timeDelta);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (mouseX != -1 && mouseY != -1) {
            rotateX((mouseY - y) * rotSpeed * timeDelta);
            rotateY((mouseX - x) * rotSpeed * timeDelta);
        }
        mouseX = x;
        mouseY = y;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseX = -1;
        mouseY = -1;
    }
}

void Player::setShaders() {
    compileShadersFromFile("./src/player/vertex.glsl", "./src/player/fragment.glsl");
}

void Player::setBuffers() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    genSphereVertices(&vertices, &vertCount);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * vertCount * sizeof(float), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
}

void Player::draw(glm::mat4 view, glm::mat4 projection, PointLight pointLight, DirectionalLight directionalLight, glm::vec3 viewPos) {
    bindProgram();
    glBindVertexArray(VAO);

    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius, radius, radius));
    GLint loc;
    loc = glGetUniformLocation(p(), "model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
    loc = glGetUniformLocation(p(), "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
    loc = glGetUniformLocation(p(), "projection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
    
    glUniform3fv(glGetUniformLocation(p(), "pointLight.position"), 1, &pointLight.position[0]);
    glUniform1f(glGetUniformLocation(p(), "pointLight.quadratic"), pointLight.quadratic);
    glUniform1f(glGetUniformLocation(p(), "pointLight.linear"), pointLight.linear);
    glUniform1f(glGetUniformLocation(p(), "pointLight.constant"), pointLight.constant);
    glUniform3fv(glGetUniformLocation(p(), "pointLight.ambient"), 1, &pointLight.ambient[0]);
    glUniform3fv(glGetUniformLocation(p(), "pointLight.diffuse"), 1, &pointLight.diffuse[0]);
    glUniform3fv(glGetUniformLocation(p(), "pointLight.specular"), 1, &pointLight.specular[0]);

    glUniform3fv(glGetUniformLocation(p(), "directionalLight.direction"), 1, &directionalLight.direction[0]);
    glUniform3fv(glGetUniformLocation(p(), "directionalLight.ambient"), 1, &directionalLight.ambient[0]);
    glUniform3fv(glGetUniformLocation(p(), "directionalLight.diffuse"), 1, &directionalLight.diffuse[0]);
    glUniform3fv(glGetUniformLocation(p(), "directionalLight.specular"), 1, &directionalLight.specular[0]);

    glUniform3fv(glGetUniformLocation(p(), "viewPos"), 1, &viewPos[0]);

    glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
