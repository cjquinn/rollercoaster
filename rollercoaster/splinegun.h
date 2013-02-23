#ifndef SPLINEGUN_H
#define SPLINEGUN_H

#include "common.h"

class SplineGun
{
public:
	SplineGun();
	~SplineGun();

	void createSpline();
	void addPoint(glm::vec3 p);

private:
	std::vector<glm::vec3> points_; 
};

#endif