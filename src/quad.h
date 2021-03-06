#ifndef QUAD_H
#define QUAD_H

#include "common.h"

class Mesh;
class Texture;

class Quad
{
public:
	Quad();
	~Quad();

	void create(float w, float h);
	void render();

private:
	Mesh *mesh_;
	Texture *texture_;
};

#endif