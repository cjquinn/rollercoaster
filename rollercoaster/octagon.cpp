#include "octagon.h"

#define GLM_SWIZZLE
#define _USE_MATH_DEFINES

#include "common.h"

#include "canvas.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "shaderprogram.h"

#include <math.h>

Octagon::Octagon() : frame_(NULL)
{}

Octagon::~Octagon()
{
	delete frame_;
}

void Octagon::create(Frame *frame)
{
	frame_ = frame;

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	vbo_.create();
	vbo_.bind();

	vertices_ = 0;
	
	for(unsigned int i = 0; i <= 8; i++) {
		float theta = (i / (float) 8) * 2 * (float) M_PI;
		
		// Frame is not working!!!!
		glm::vec3 p(glm::cos(theta) * glm::vec3(0,1,0) + glm::sin(theta) * glm::vec3(0,0,1));
		glm::vec3 n(0.0f, 1.0f, 0.0f);
		
		vertices_++;

		vbo_.addData(&p, sizeof(glm::vec3));
		vbo_.addData(&n, sizeof(glm::vec3));
	}

	vbo_.uploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2*sizeof(glm::vec3);
	
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)));
}

void Octagon::render()
{
	ShaderProgram *main = Canvas::instance().shader_programs();
	main->use();
	main->setUniform("toonify", false);
	main->setUniform("texture_fragment", false);
	
	Lighting::white();

	// Set up a matrix stack
  glutil::MatrixStack modelview = Canvas::instance().modelview();

	modelview.push();
		modelview.translate(frame_->p());

	  main->setUniform("matrices.modelview", modelview.top());
		glBindVertexArray(vao_);

		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices_);
	modelview.pop();
}

void Octagon::release()
{
	glDeleteVertexArrays(1, &vao_);
	vbo_.release();
}