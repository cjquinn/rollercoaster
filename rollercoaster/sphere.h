#ifndef SPHERE_H
#define SPHERE_H

#include "vbo.h"

// Class for generating a unit sphere
class Sphere
{
public:
  void create(glm::vec3 position, int slices, int stacks);
  void render();
  void release();

private:
	glm::vec3 position_;

  UINT vao_;
  VBO vbo_;
  int triangles_;
};

#endif
