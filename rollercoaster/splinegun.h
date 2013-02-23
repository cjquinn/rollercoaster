#ifndef SPLINEGUN_H
#define SPLINEGUN_H

#include "common.h"

class Sphere;

class SplineGun
{
public:
	SplineGun();
	~SplineGun();

	void createSpline();
	void addPoint(glm::vec3 p);

	void render();

private:
	std::vector<Sphere*> points_; 
};

#endif