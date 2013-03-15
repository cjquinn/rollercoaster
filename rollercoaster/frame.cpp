#include "frame.h"

Frame::Frame(glm::vec3 p, glm::vec3 q) : p_(p), q_(q)
{
	t_ = q_ - p_;

	n_ = glm::normalize(glm::cross(t_, glm::vec3(0.0f, 1.0f, 0.0f)));
	b_ = glm::normalize(glm::cross(n_, t_));
}

Frame::~Frame()
{
}

glm::vec3 Frame::b()
{
	return b_;
}

glm::vec3 Frame::n()
{

	return n_;
}

glm::vec3 Frame::p()
{
	return p_;
}

glm::vec3 Frame::t()
{
	return glm::normalize(t_);
}