#ifndef OCTAGON_H
#define OCTAGON_H

#include "vbo.h"

class Frame;

// Class for generating a unit Octagon
class Octagon
{
public:
	Octagon();
	~Octagon();

	void create(Frame *frame);
	
	void render();
	void release();
	
private:
	GLuint vao_;
	VBO vbo_;
	unsigned int vertices_;

	Frame *frame_;
};

#endif