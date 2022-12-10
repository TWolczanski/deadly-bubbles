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

void Aquarium::draw(glm::mat4 view, glm::mat4 projection, PointLight pointLight, DirectionalLight directionalLight, glm::vec3 viewPos) {
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

    glDrawArrays(GL_TRIANGLES, 0, 36);
}
