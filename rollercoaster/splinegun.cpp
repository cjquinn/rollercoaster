#include "splinegun.h"

#include "sphere.h"
#include "spline.h"

SplineGun::SplineGun() : spline_(NULL)
{
}

SplineGun::~SplineGun()
{
	while(!spheres_.empty()) {
		delete spheres_.back();
		spheres_.pop_back();
	}

	delete spline_;
}

void SplineGun::createSpline()
{
	
}

void SplineGun::addPoint(glm::vec3 p)
{
	Sphere *sphere = new Sphere;
	sphere->create(p, 25, 25);
	spheres_.push_back(sphere);
	points_.push_back(p);

	if(points_.size() > 4) {
		spline_ = new Spline;
		spline_->create(points_, 100);
	}
}

void SplineGun::render()
{
	for(std::vector<Sphere*>::iterator sphere = spheres_.begin(); sphere != spheres_.end(); ++sphere) {
		(*sphere)->render();
	}

	if(spline_) {
		spline_->render();
	}
}