#ifndef SPLINEGUN_H
#define SPLINEGUN_H

#include "common.h"

class Point;
class Spline;

class SplineGun
{
public:
	SplineGun();
	~SplineGun();

	void addPoint(glm::vec3 p);

	void render();

	Spline *spline();

private:
	std::vector<Point*> points_;
	Spline *spline_;
};

#endif