#ifndef TRACK_H
#define TRACK_H

#include "common.h"

class Circle;
class Mesh;
class Spline;
class Support;

class Track
{
public:
	Track();
	~Track();

	void create(std::string track);
	void render(); 

	Spline* spline();

private:
	std::vector<Circle*> circles_;
	std::vector<Support*> supports_;

	Mesh *mesh_;
	Spline *spline_;
};

#endif