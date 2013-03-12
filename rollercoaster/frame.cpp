#include "frame.h"

Frame::Frame(glm::vec3 p, glm::vec3 q) : p_(p), t_(q-p), n_(glm::cross(t_, glm::vec3(0,1,0))), b_(glm::cross(n_, t_))
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
	return glm::normalize(t_);
}