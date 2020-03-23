#include "circle.h"

#include "canvas.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "shaderprogram.h"

Circle::Circle() : frame_(NULL)
{}

Circle::~Circle()
{
	glDeleteVertexArrays(1, &vao_);
	vbo_.release();
	delete frame_;
}

void Circle::create(glm::vec3 p, int samples, float scale) 
{
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	vbo_.create();
	vbo_.bind();

	for (int i = 0; i <= samples; i++) {
		float theta = (i / (float) samples) * 2 * (float) M_PI;
		p += glm::vec3(glm::cos(theta), 0.0f, glm::sin(theta));
		vertices_.push_back(p);
	}

	for (unsigned int i = 0; i < vertices_.size(); i++) {
		vbo_.addData(&vertices_.at(i), sizeof(glm::vec3));
	}

	vbo_.uploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
}

void Circle::create(Frame *frame, int samples, float scale)
{
	frame_ = frame;

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	vbo_.create();
	vbo_.bind();

	for (int i = 0; i <= samples; i++) {
		float theta = (i / (float) samples) * 2 * (float) M_PI;
		glm::vec3 p(frame_->p() + scale * glm::cos(theta) * frame_->n() + scale * glm::sin(theta) * frame_->b());
		vertices_.push_back(p);
	}

	for (unsigned int i = 0; i < vertices_.size(); i++) {
		vbo_.addData(&vertices_.at(i), sizeof(glm::vec3));
	}

	vbo_.uploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
}

void Circle::render()
{
	ShaderProgram *main = Canvas::instance().shader_programs(0);
	main->use();
	main->setUniform("toonify", false);
	main->setUniform("texture_fragment", false);
	
	Lighting::white();

	// Set up a matrix stack
    glutil::MatrixStack modelview = Canvas::instance().modelview();

	modelview.push();
	
    main->setUniform("matrices.modelview", modelview.top());
	glBindVertexArray(vao_);
	glPointSize(1.0f);
	glDrawArrays(GL_POINTS, 0, vertices_.size());

	modelview.pop();
}

std::vector<glm::vec3> Circle::vertices()
{
	return vertices_;
}