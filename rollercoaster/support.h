#ifndef SUPPORT_H
#define SUPPORT_H

#include "mesh.h"

class Support
{
public:
	Support();
	~Support();

	void create(glm::vec3 p);

	void render();

private:
	Mesh mesh_;
};

#endif