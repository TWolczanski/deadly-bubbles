#include <aquarium.hpp>
#include <constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Aquarium::Aquarium() : AGLDrawable() {
    setShaders();
    setBuffers();
}

Aquarium::~Aquarium() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Aquarium::setShaders() {
    compileShadersFromFile("./src/aquarium/vertex.glsl", "./src/aquarium/fragment.glsl");
}

void Aquarium::setBuffers() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // float vert[] = {
    //     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    //     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    //     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    //      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    //     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    //     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    //      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    // };

    float vert[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
}

void Aquarium::draw(glm::mat4 view, glm::mat4 projection, PointLight pointLights[], int pointLightCount, glm::vec3 viewPos) {
    bindProgram();
    glBindVertexArray(VAO);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(AQUARIUM_SIZE_X / 2, AQUARIUM_SIZE_Y / 2, AQUARIUM_SIZE_Z / 2));
    model = glm::scale(model, glm::vec3(AQUARIUM_SIZE_X, AQUARIUM_SIZE_Y, AQUARIUM_SIZE_Z));

    GLint loc;
    loc = glGetUniformLocation(p(), "model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
    loc = glGetUniformLocation(p(), "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
    loc = glGetUniformLocation(p(), "projection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

    for (int i = 0; i < pointLightCount; i++) {
        glUniform3fv(glGetUniformLocation(p(), (std::string("pointLights[") + std::to_string(i) + std::string("].position")).c_str()), 1, &(pointLights[i].position)[0]);
        glUniform1f(glGetUniformLocation(p(), (std::string("pointLights[") + std::to_string(i) + std::string("].quadratic")).c_str()), pointLights[i].quadratic);
        glUniform1f(glGetUniformLocation(p(), (std::string("pointLights[") + std::to_string(i) + std::string("].linear")).c_str()), pointLights[i].linear);
        glUniform1f(glGetUniformLocation(p(), (std::string("pointLights[") + std::to_string(i) + std::string("].constant")).c_str()), pointLights[i].constant);
        glUniform3fv(glGetUniformLocation(p(), (std::string("pointLights[") + std::to_string(i) + std::string("].ambient")).c_str()), 1, &(pointLights[i].ambient)[0]);
        glUniform3fv(glGetUniformLocation(p(), (std::string("pointLights[") + std::to_string(i) + std::string("].diffuse")).c_str()), 1, &(pointLights[i].diffuse)[0]);
        glUniform3fv(glGetUniformLocation(p(), (std::string("pointLights[") + std::to_string(i) + std::string("].specular")).c_str()), 1, &(pointLights[i].specular)[0]);
    }

    glUniform3fv(glGetUniformLocation(p(), "viewPos"), 1, &viewPos[0]);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}
