#include <bubbles.hpp>
#include <sphere.hpp>
#include <constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Bubbles::Bubbles(int count, float speed, float growthRate) : count(count), speed(speed), growthRate(growthRate), maxRadius(0.1) {
    setShaders();
    setBuffers();
}

Bubbles::~Bubbles() {
    delete[] positions;
    delete[] radii;
    delete[] colors;
    delete[] vertices;
    delete[] modelBuffer;
    delete[] colorBuffer;
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &colorVBO);
    glDeleteBuffers(1, &modelVBO);
    glDeleteVertexArrays(1, &VAO);
}

void Bubbles::setShaders() {
    compileShadersFromFile("./src/bubbles/vertex.glsl", "./src/bubbles/fragment.glsl");
}

void Bubbles::setBuffers()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    genSphereVertices(&vertices, &vertCount);
    positions = new glm::vec3[count];
    radii = new float[count];
    colors = new glm::vec3[count];
    colorBuffer = new glm::vec3[count];
    modelBuffer = new glm::mat4[count];
    for (int i = 0; i < count; i++) {
        positions[i] = glm::vec3(0.0f, AQUARIUM_SIZE_Y + 1, 0.0f);
        radii[i] = 0.0;
        colors[i] = glm::vec3(0.0f);
        modelBuffer[i] = glm::mat4(1.0f);
        colorBuffer[i] = glm::vec3(0.0f);
    }

    glGenBuffers(1, &sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * vertCount * sizeof(float), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), &colorBuffer[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glGenBuffers(1, &modelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), &modelBuffer[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
}

void Bubbles::draw(glm::mat4 view, glm::mat4 projection, PointLight pointLight, glm::vec3 viewPos, double time, double timeDelta) {
    bindProgram();
    glBindVertexArray(VAO);

    // attempt to create a new bubble
    int index = ((int)((time - (long)time) * 1000)) % count;
    bool created = false;
    if (positions[index].y + radii[index] >= AQUARIUM_SIZE_Y) {
        float r = ((float)rand() / RAND_MAX) * 0.02 + 0.005;
        float x = ((float)rand() / (RAND_MAX / (AQUARIUM_SIZE_X - maxRadius - maxRadius))) + maxRadius;
        float z = ((float)rand() / (RAND_MAX / (AQUARIUM_SIZE_Z - maxRadius - maxRadius))) + maxRadius;
        positions[index] = glm::vec3(x, r, z);
        radii[index] = r;
        colors[index] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        created = true;
    }

    int j = 0;
    for (int i = 0; i < count; i++) {
        if (!(i == index && created)) {
            positions[i].y += speed * timeDelta;
            radii[i] = std::min((float)(radii[i] + growthRate * timeDelta), maxRadius);
        }
        // only the bubbles that haven't made it to the top of the aquarium are drawn
        if (positions[i].y + radii[i] < AQUARIUM_SIZE_Y) {
            colorBuffer[j] = colors[i];
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            model = glm::scale(model, glm::vec3(radii[i], radii[i], radii[i]));
            modelBuffer[j] = model;
            j++;
        }
    }
    int instanceCount = j;

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instanceCount * sizeof(glm::vec3), &colorBuffer[0]);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instanceCount * sizeof(glm::mat4), &modelBuffer[0]);

    GLint loc;
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
    glUniform1f(glGetUniformLocation(p(), "pointLight.shininess"), pointLight.shininess);

    glUniform3fv(glGetUniformLocation(p(), "viewPos"), 1, &viewPos[0]);

    glDrawArraysInstanced(GL_TRIANGLES, 0, vertCount, instanceCount);
}