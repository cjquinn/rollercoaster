#include "point.h"

#include "canvas.h"
#include "lighting.h"
#include "shaderprogram.h"
#include "sphere.h"

Point::Point() : sphere_(NULL), colour_(1.0f, 0.0f, 0.0f)
{
}

Point::~Point()
{
	delete sphere_;
}

void Point::create(glm::vec3 position)
{
	position_ = position;

	sphere_ = new Sphere;
	sphere_->create(25, 25);
}

void Point::render()
{
	ShaderProgram *main = Canvas::instance().shader_programs();
	main->use();
	main->setUniform("toonify", false);
	main->setUniform("texture_fragment", false);
	
	// Set up a matrix stack
  glutil::MatrixStack modelview = Canvas::instance().modelview();

	main->setUniform("material.ambient", colour_);

	modelview.push();
		modelview.translate(position_);

	  main->setUniform("matrices.modelview", modelview.top());

		sphere_->render();
	modelview.pop();
}

// Getters and setters
glm::vec3 Point::position()
{
	return position_;
}

void Point::setPosition(glm::vec3 position)
{
	position_ = position;
}

void Point::setColour(glm::vec3 colour)
{
	colour_ = colour;
}