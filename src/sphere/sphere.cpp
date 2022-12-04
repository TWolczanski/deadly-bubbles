#include <sphere.hpp>
#include <deque>

// based on https://stackoverflow.com/a/7687312
void genSphereVertices(float **vertices, int *count)
{
    // octahedron
    float vert[] = {
        -1.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  0.0f, -1.0f,

         1.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  0.0f, -1.0f,

         1.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  0.0f,  1.0f,

        -1.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  0.0f,  1.0f,

        -1.0f,  0.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f,  0.0f, -1.0f,

         1.0f,  0.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f,  0.0f, -1.0f,

         1.0f,  0.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f,  0.0f,  1.0f,

        -1.0f,  0.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f,  0.0f,  1.0f,
    };

    std::deque<glm::vec3> vertQueue;
    for (int i = 0; i < 24; i++) {
        vertQueue.push_back(glm::vec3(vert[i * 3], vert[i * 3 + 1], vert[i * 3 + 2]));
    }

    const int STEPS = 4;

    // triangle division
    for (int i = 0; i < STEPS; i++) {
        int n = vertQueue.size() / 3;
        for (int j = 0; j < n; j++) {
            glm::vec3 p1 = vertQueue.front();
            vertQueue.pop_front();
            glm::vec3 p2 = vertQueue.front();
            vertQueue.pop_front();
            glm::vec3 p3 = vertQueue.front();
            vertQueue.pop_front();
            glm::vec3 p4 = (p1 + p2) / 2.0f;
            glm::vec3 p5 = (p1 + p3) / 2.0f;
            glm::vec3 p6 = (p2 + p3) / 2.0f;

            normalizePoint(p1);
            normalizePoint(p2);
            normalizePoint(p3);
            normalizePoint(p4);
            normalizePoint(p5);
            normalizePoint(p6);

            vertQueue.push_back(p1);
            vertQueue.push_back(p4);
            vertQueue.push_back(p5);

            vertQueue.push_back(p4);
            vertQueue.push_back(p2);
            vertQueue.push_back(p6);

            vertQueue.push_back(p5);
            vertQueue.push_back(p6);
            vertQueue.push_back(p3);

            vertQueue.push_back(p4);
            vertQueue.push_back(p6);
            vertQueue.push_back(p5);
        }
    }

    *count = vertQueue.size();
    *vertices = new float[*count * 3];
    for (int i = 0; i < *count; i++) {
        glm::vec3 v = vertQueue.front();
        vertQueue.pop_front();
        (*vertices)[3 * i] = v.x;
        (*vertices)[3 * i + 1] = v.y;
        (*vertices)[3 * i + 2] = v.z;
    }
}

void normalizePoint(glm::vec3 &p) {
    float dist = glm::distance(p, glm::vec3(0.0f, 0.0f, 0.0f));
    p.x /= dist;
    p.y /= dist;
    p.z /= dist;
}