#include "vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 colour, glm::vec2 texture) :
  position(position), normal(normal), colour(colour), texture(texture)
{
}
