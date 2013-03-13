#include "frame.h"

Frame::Frame(glm::vec3 p, glm::vec3 q) : 
	p_(p), q_(q), t_(glm::normalize(q_ - p_)), 
	n_(glm::normalize(glm::cross(t_, glm::vec3(0, 1, 0)))), b_(glm::normalize(glm::cross(n_, t_)))
{
}

Frame::~Frame()
{
}

glm::vec3 Frame::b()
{
	return glm::normalize(b_);
}

glm::vec3 Frame::n()
{

	return glm::normalize(n_);
}

glm::vec3 Frame::p()
{
	return p_;
}

glm::vec3 Frame::t()
{
	return t_;
}