#ifndef PENGUINS_H
#define PENGUINS_H

#include "common.h"

class ObjModel;

class Penguins
{
public:
	Penguins();
	~Penguins();

	void create(int n, glm::vec3 origin);

	void render();

	void setNight();

private:
	std::vector<glm::vec3> positions_;
	std::vector<glm::mat4> rotations_;
	std::vector<float> scales_;

	glm::vec3 origin_;

	bool night_;

	ObjModel *model_;
};

#endif