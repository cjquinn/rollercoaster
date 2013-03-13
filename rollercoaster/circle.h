#ifndef CIRCLE_H
#define CIRCLE_H

#include "vbo.h"

#include "common.h"

class Frame;

class Circle
{
public:
	Circle();
	~Circle();

	void create(Frame *frame, int samples, float scale);
	
	void render();
	void release();

	std::vector<glm::vec3> vertices();
	
private:
	GLuint vao_;
	VBO vbo_;

	std::vector<glm::vec3> vertices_;

	Frame *frame_;
};

#endif