#include "track.h"

#include "frame.h"
#include "octagon.h"

Track::Track()
{}

Track::~Track()
{
	while(!octagons_.empty()) {
		delete octagons_.back();
		octagons_.pop_back();
	}
}

void Track::create(Spline *spline) 
{
	for (unsigned int i = 0; i < spline->sampled_points().size() - 1; i++) {
		octagons_.push_back(new Octagon); 
		octagons_.at(i)->create(new Frame(spline->sampled_points().at(i), spline->sampled_points().at(i+1)));
	}
}

void Track::render()
{
	for (unsigned int i = 0; i < octagons_.size(); i++) {
		octagons_.at(i)->render();	
	}
}