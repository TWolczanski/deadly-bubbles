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
    delete[] models;
    delete[] vertices;
}

void Bubbles::setShaders() {
    compileShadersFromFile("./src/bubbles/vertex.glsl", "./src/bubbles/fragment.glsl");
}

void Bubbles::setBuffers()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    genSphereVertices(&vertices, &vertCount);

    glGenBuffers(1, &sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * vertCount * sizeof(float), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    positions = new glm::vec3[count];
    radii = new float[count];
    models = new glm::mat4[count];
    for (int i = 0; i < count; i++) {
        positions[i] = glm::vec3(0.0f, AQUARIUM_SIZE_Y + 1, 0.0f);
        radii[i] = 0.0;
        models[i] = glm::mat4(1.0f);
    }

    glGenBuffers(1, &modelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
}

void Bubbles::draw(glm::mat4 view, glm::mat4 projection, double time, double timeDelta) {
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
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            model = glm::scale(model, glm::vec3(radii[i], radii[i], radii[i]));
            models[j] = model;
            j++;
        }
    }
    int instanceCount = j;

    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instanceCount * sizeof(glm::mat4), &models[0]);

    GLint loc;
    loc = glGetUniformLocation(p(), "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
    loc = glGetUniformLocation(p(), "projection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArraysInstanced(GL_TRIANGLES, 0, vertCount, instanceCount);
}