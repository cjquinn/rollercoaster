#ifndef SPLINEGUN_H
#define SPLINEGUN_H

#include "common.h"

class Sphere;
class Spline;

class SplineGun
{
public:
	SplineGun();
	~SplineGun();

	void createSpline();
	void addPoint(glm::vec3 p);

	void render();

private:
	std::vector<glm::vec3> points_;
	std::vector<Sphere*> spheres_; 

	Spline *spline_;
};

#endif