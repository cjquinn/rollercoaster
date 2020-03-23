#ifndef CIRCLE_H
#define CIRCLE_H

#include "common.h"
#include "vbo.h"

class Frame;

class Circle
{
public:
	Circle();
	~Circle();

	void create(glm::vec3 p, int samples, float scale);
	void create(Frame *frame, int samples, float scale);
	
	void render();

	std::vector<glm::vec3> vertices();
	
private:
	GLuint vao_;
	VBO vbo_;

	std::vector<glm::vec3> vertices_;

	Frame *frame_;
};

#endif