#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "common.h"

class Billboard
{
public:
	Billboard();
	~Billboard();

	void create(glm::vec3 p, std::string texture);
	void render();
private:

};

#endif