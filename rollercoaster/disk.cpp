#define GLM_SWIZZLE
#define _USE_MATH_DEFINES

#include "common.h"
#include "disk.h"

#include <math.h>

Disk::Disk()
{}

Disk::~Disk()
{}

void Disk::create(std::string directory, std::string file, unsigned int samples, Frame frame)
{
	texture_.load(directory+file);
	texture_.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
	texture_.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture_.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	vbo_.create();
	vbo_.bind();

	vertices_ = 0;
	
	for(unsigned int i = 0; i <= samples; i++) {
		float theta = (i / (float) samples) * 2 * (float) M_PI;
		glm::vec3 p(glm::cos(theta), 0.0f, glm::sin(theta));
		glm::vec2 t(p.xz() / 2.0f + glm::vec2(0.5f));
		glm::vec3 n(0.0f, 1.0f, 0.0f);
		vertices_++;

		vbo_.addData(&p, sizeof(glm::vec3));
		vbo_.addData(&t, sizeof(glm::vec2));
		vbo_.addData(&n, sizeof(glm::vec3));
	}

	vbo_.uploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);
	
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
}

void Disk::render()
{
	glBindVertexArray(vao_);
	texture_.bind();

	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices_);
}

void Disk::release()
{
	texture_.release();
	glDeleteVertexArrays(1, &vao_);
	vbo_.release();
}