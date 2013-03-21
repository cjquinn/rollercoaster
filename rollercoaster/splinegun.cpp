#include "splinegun.h"

#include <iostream>
#include <fstream>

#include "spline.h"

SplineGun::SplineGun() : spline_(NULL)
{
}

SplineGun::~SplineGun()
{
	delete spline_;
}

void SplineGun::addPoint(glm::vec3 p)
{
	points_.push_back(p);

	if(points_.size() > 3) {
		spline_ = new Spline;
		spline_->create(points_);
	}
}

void SplineGun::render()
{
	if(spline_) {
		spline_->render();
	}
}

void SplineGun::save()
{
	std::ofstream file;
	file.open("resources\\track\\main.txt");

	for (unsigned int i = 0; i < points_.size(); i++) {
		file << points_.at(i).x;
		file << ",";
		file << points_.at(i).y;
		file << ",";
		file << points_.at(i).z;
		file << "\n";
	}

	file.close();
}

Spline *SplineGun::spline()
{
	return spline_;
}

