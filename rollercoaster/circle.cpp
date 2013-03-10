#include "circle.h"

#include "common.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "canvas.h"
#include "shaderprogram.h"
#include "lighting.h"

Circle::Circle() : vertices_(0)
{
}

Circle::~Circle()
{
	glDeleteVertexArrays(1, &vao_);
  vbo_.release();
}

void Circle::create(int samples)
{
	glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  vbo_.create();
  vbo_.bind();

	vertices_.push_back(glm::vec3(0.0f));

	for(int i = 0; i <= samples; i++) {
		float theta = (i / samples) * 2 * (float) M_PI;
		vertices_.push_back(glm::vec3(glm::cos(theta), 0.0f, glm::sin(theta)));
	}

	glm::vec3 normal(0,1,0);

	for (int i = 0; i < (int) vertices_.size(); ++i) {
    vbo_.addData(&vertices_[i], sizeof(glm::vec3));
		vbo_.addData(&normal, sizeof(glm::vec3));
  }

	vbo_.uploadDataToGPU(GL_STATIC_DRAW);

  GLsizei stride = 2 * sizeof(glm::vec3);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Normal positions
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *) sizeof(stride));
}

void Circle::render()
{
	ShaderProgram *main = Canvas::instance().shader_programs();
	main->use();
	main->setUniform("toonify", false);
	main->setUniform("texture_fragment", false);
	
	Lighting::white();

	main->setUniform("material.ambient", glm::vec3(1.0f, 0.0f, 0.0f));

	// Set up a matrix stack
  glutil::MatrixStack modelview = Canvas::instance().modelview();

	modelview.push();
	  modelview.translate(0.0f, 0.0f, 0.0f);
	
		main->setUniform("matrices.modelview", modelview.top());
		
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices_.size());
	modelview.pop();
}