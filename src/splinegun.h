#ifndef SPLINEGUN_H
#define SPLINEGUN_H

#include "common.h"

class Spline;

class SplineGun
{
public:
	SplineGun();
	~SplineGun();

	void addPoint(glm::vec3 p);

	void render();

	Spline *spline();

	void save();

private:
	std::vector<glm::vec3> points_;
	Spline *spline_;
};

#endif