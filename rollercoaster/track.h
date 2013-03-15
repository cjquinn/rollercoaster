#ifndef TRACK_H
#define TRACK_H

#include "mesh.h"
#include "spline.h"

class Circle;
class Support;
class Texture;

class Track
{
public:
	Track();
	~Track();

	void create(Spline *spline);
	void render();

private:
	Mesh mesh_;

	std::vector<Circle*> circles_;
	std::vector<Support*> supports_;

	Texture *texture_;
};

#endif