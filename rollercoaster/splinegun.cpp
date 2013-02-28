#include "splinegun.h"

#include "point.h"
#include "spline.h"

SplineGun::SplineGun() : render_(true), spline_(NULL)
{
}

SplineGun::~SplineGun()
{
	while(!points_.empty()) {
		delete points_.back();
		points_.pop_back();
	}

	delete spline_;
}

void SplineGun::addPoint(glm::vec3 p)
{
	Point *point = new Point;
	point->create(p);
	
	if(points_.size() > 0) {
		points_.back()->setColour(glm::vec3(1.0f));
	}
	
	points_.push_back(point);

	std::vector<glm::vec3> points;

	for(std::vector<Point*>::iterator point = points_.begin(); point != points_.end(); ++point) {
		points.push_back((*point)->position());
	}

	if(points_.size() > 3) {
		spline_ = new Spline;
		spline_->create(points);
	}
}

void SplineGun::render()
{
	if(render_) {
		for(std::vector<Point*>::iterator point = points_.begin(); point != points_.end(); ++point) {
			(*point)->render();
		}
	}
		if(spline_) {
			spline_->render();
		}
}

void SplineGun::setRender(bool render)
{
	render_ = render;
}

Spline *SplineGun::spline()
{
	return spline_;
}

