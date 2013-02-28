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

	void setRender(bool render);
	Spline *spline();

private:
	std::vector<Point*> points_;

	bool render_;

	Spline *spline_;
};

#endif