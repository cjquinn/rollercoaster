#include "splinegun.h"
#include "sphere.h"

SplineGun::SplineGun()
{
}

SplineGun::~SplineGun()
{
	while(!points_.empty()) {
		delete points_.back();
		points_.pop_back();
	}
}

void SplineGun::createSpline()
{
}

void SplineGun::addPoint(glm::vec3 p)
{
	Sphere *sphere = new Sphere;
	sphere->create(p, 25, 25);
	points_.push_back(sphere);
}

void SplineGun::render()
{
	for(std::vector<Sphere*>::iterator point = points_.begin(); point != points_.end(); ++point) {
		(*point)->render();
	}
}