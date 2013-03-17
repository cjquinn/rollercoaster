#ifndef CUBOID_H
#define CUBOID_H

#include "common.h"

class Cuboid
{
public:
	Cuboid();
	~Cuboid();

	void create(float w, float h, float d, glm::vec3 p);

	std::vector<glm::vec3> sampled_points();

private:
	std::vector<glm::vec3> sampled_points_;
};

#endif