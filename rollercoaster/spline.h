#ifndef SPLINE_H
#define SPLINE_H

#include "common.h"
#include "vbo.h"

class Spline
{
public:
	Spline();
	~Spline();
	glm::vec3 interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t);
	
	void create(const std::vector<glm::vec3> &points);
	void render();

	void addPoint(glm::vec3 p);

	glm::vec3 pointAt(float d);

private:
	void computeLength();

	UINT vao_;
  VBO vbo_;

	std::vector<float> distances_;
	std::vector<glm::vec3> points_;
	std::vector<glm::vec3> sampled_points_;
};

#endif
