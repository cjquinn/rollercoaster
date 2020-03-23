#include "spline.h"

#include "canvas.h"
#include "lighting.h"
#include "matrixstack.h"
#include "shaderprogram.h"

Spline::Spline() {}

Spline::~Spline() {}

glm::vec3 Spline::interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

	return a + b*t + c*t2 + d*t3;
}

void Spline::computeLength()
{
	distances_.clear();

	float length = 0.0f;
	distances_.push_back(length);

	for (unsigned int i = 1; i < points_.size(); i++) {
		length += glm::distance(points_[i-1], points_[i]);
		distances_.push_back(length);
	}

	length += glm::distance(points_[points_.size()-1], points_[0]);
	distances_.push_back(length);
}

glm::vec3 Spline::pointAt(float d)
{
	float total_length = distances_[distances_.size()-1];

	float length = d - (int) (d / total_length) * total_length;

	int segment;

	for (unsigned int i = 0; i < distances_.size(); i++) {
		if (length >= distances_[i] && length < distances_[i+1]) {
			segment = i;
			break;
		}
	}

	float segment_length =  distances_[segment + 1] - distances_[segment];	
	float t = (length - distances_[segment]) / segment_length;	
	
	int p0 = ((segment - 1) + points_.size()) % points_.size();
	int p1 = segment;
	int p2 = (segment + 1) % points_.size();
	int p3 = (segment + 2) % points_.size();

	return interpolate(points_[p0], points_[p1], points_[p2], points_[p3], t);
}

void Spline::create(const std::vector<glm::vec3> &points)
{
	points_ = points;
	sampled_points_.clear();

	computeLength();
	float total_length = distances_[distances_.size() - 1];

	float spacing = 4;
	float n = total_length / spacing;


	for (int i = 0; i < n; i++) {
		sampled_points_.push_back(pointAt(i * spacing));
	}

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	vbo_.create();
	vbo_.bind();

	glm::vec2 texture_coord(0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	for (unsigned int i = 0; i < sampled_points_.size(); i++) {
		glm::vec3 v = sampled_points_[i];
		vbo_.addData(&v, sizeof(glm::vec3));
		vbo_.addData(&texture_coord, sizeof(glm::vec2));
		vbo_.addData(&normal, sizeof(glm::vec3));
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

void Spline::render()
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

    glLineWidth(2.5f);

    glDrawArrays(GL_LINE_LOOP, 0, sampled_points_.size());
	modelview.pop();
}

std::vector<glm::vec3> Spline::points() 
{
	return points_;
}

std::vector<glm::vec3> Spline::sampled_points()
{
	return sampled_points_;
}
