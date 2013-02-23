#ifndef VERTEX_H
#define VERTEX_H

#include "include/glm/gtc/type_ptr.hpp"

class Vertex
{
public:
  Vertex();
  Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 colour, glm::vec2 texture);

	glm::vec2 texture;
  glm::vec3 position, normal, colour;
};

#endif
