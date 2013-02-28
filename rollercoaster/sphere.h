#ifndef SPHERE_H
#define SPHERE_H

#include "vbo.h"

// Class for generating a unit sphere
class Sphere
{
public:
	Sphere();
	~Sphere();

  void create(int slices, int stacks);
  void render();

private:
  UINT vao_;
  VBO vbo_;
  int triangles_;
};

#endif
