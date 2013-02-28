#ifndef POINT_H
#define POINT_H

#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

class Sphere;

class Point
{
public:
	Point();
	~Point();

	void create(glm::vec3 position);
	void render();

	void setPosition(glm::vec3 position);
	glm::vec3 position();

	void setColour(glm::vec3 colour);

private:
	glm::vec3 position_;
	glm::vec3 colour_;

	Sphere *sphere_;
};

#endif
