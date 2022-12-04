#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>

void genSphereVertices(float **vertices, int *count);
void normalizePoint(glm::vec3 &p);

#endif