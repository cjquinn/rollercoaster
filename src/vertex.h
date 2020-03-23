#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class Vertex
{
public:
    Vertex();
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture);

    glm::vec2 texture;
    glm::vec3 position, normal;
};

#endif
