#ifndef FRAME_H
#define FRAME_H

#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

class Frame
{
public:
	Frame(glm::vec3 p, glm::vec3 q, glm::vec3 y);
	~Frame();

	glm::vec3 b();
	glm::vec3 n();
	glm::vec3 p();
	glm::vec3 t();

private:
	glm::vec3 p_;
	glm::vec3 q_;
	glm::vec3 n_;
	glm::vec3 b_;
	glm::vec3 t_;
};

#endif