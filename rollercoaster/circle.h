#ifndef CIRCLE_H
#define CIRCLE_H

#include "vbo.h"

class Circle
{
public:
	Circle();
	~Circle();

	void create(int samples);
	void render();

private:
  UINT vao_;
  VBO vbo_;
  std::vector<glm::vec3> vertices_;
};

#endif