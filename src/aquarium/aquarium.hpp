#ifndef AQUARIUM_H
#define AQUARIUM_H

#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include <AGL3Drawable.hpp>
#include <light.hpp>

class Aquarium : public AGLDrawable
{
    GLuint VAO;
    GLuint VBO;

public:
    Aquarium();
    ~Aquarium();
    void draw(glm::mat4 view, glm::mat4 projection, PointLight pointLight, glm::vec3 viewPos);
    
private:
    void setShaders();
    void setBuffers();
};

#endif