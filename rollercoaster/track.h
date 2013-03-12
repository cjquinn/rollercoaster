#ifndef TRACK_H
#define TRACK_H

#include "spline.h"

class Octagon;

class Track
{
public:
	Track();
	~Track();

	void create(Spline *spline);
	void render();

private:
	std::vector<Octagon*> octagons_;
};

#endif