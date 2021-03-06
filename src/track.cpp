#include "track.h"

#include <iostream>
#include <fstream>

#include "canvas.h"
#include "circle.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "mesh.h"
#include "shaderprogram.h"
#include "spline.h"
#include "support.h"
#include "texture.h"
#include "vertex.h"

Track::Track() : mesh_(NULL), spline_(NULL), texture_(NULL) {}

Track::~Track()
{
	while(!supports_.empty()) {
    	delete supports_.back();
    	supports_.pop_back();
  	}

	while(!circles_.empty()) {
    	delete circles_.back();
    	circles_.pop_back();
  	}

	delete mesh_;
	delete spline_;
	delete texture_;
}

void Track::create(std::string track) 
{
	std::vector<glm::vec3> points;

	std::string line;
	std::ifstream file(track);

	while (std::getline(file, line)) {
		std::vector<float> coords;
		std::stringstream ss(line);
		std::string coord;

		while (std::getline(ss, coord, ',')) {
			coords.push_back(std::stof(coord));
		}

		points.push_back(glm::vec3(coords.at(0), coords.at(1), coords.at(2)));
	}

	mesh_ = new Mesh;

	spline_ = new Spline;
	spline_->create(points);

	texture_ = new Texture;
	texture_->load(ROOT_DIR + std::string("/resources/textures/track.jpg"));
	texture_->setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
  	texture_->setSamplerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
  	texture_->setSamplerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	int samples = 5;

	for (unsigned int i = 0; i < spline_->sampled_points().size() - 1; i++) {
		Circle *circle = new Circle;
		circle->create(new Frame(spline_->sampled_points().at(i), spline_->sampled_points().at(i+1)), samples, 2.0f);
		circles_.push_back(circle);

		if (i % (int) (spline_->sampled_points().size() / 60) == 0) {
			Support *support = new Support;
			support->create(spline_->sampled_points().at(i));
			supports_.push_back(support);
		}
	}

	for (unsigned int j = 0; j < circles_.size(); ++j) {
		for (int i = 0; i < samples; ++i) {
			Vertex v(circles_.at(j)->vertices().at(i), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
      		vertices.push_back(v);
		}
	}

	for (int i = 0; i < samples; ++i) {
		for (unsigned int j = 0; j < circles_.size(); ++j) {
      		int index = i + j * samples;
		  
			if (j < circles_.size() - 1) {
				triangles.push_back((index + 1 + samples) % vertices.size());
				triangles.push_back((index + 1) % vertices.size());
				triangles.push_back((index) % vertices.size()); 

				triangles.push_back((index + samples) % vertices.size());
				triangles.push_back((index + 1 + samples) % vertices.size());
				triangles.push_back((index) % vertices.size());
			} else {
     			triangles.push_back((1 + i) % vertices.size()); 
				triangles.push_back((index + 1) % vertices.size());
				triangles.push_back((index) % vertices.size());

				triangles.push_back((i) % vertices.size());
				triangles.push_back((1 + i) % vertices.size());
				triangles.push_back((index) % vertices.size());
			}
    	}
  	}

	mesh_->create(vertices, triangles, true);
}

void Track::render()
{
	ShaderProgram *main = Canvas::instance().shader_programs(0);
	main->use();
	main->setUniform("toonify", true);
	main->setUniform("texture_fragment", true);

	texture_->bind();
	
	// Set up a matrix stack
  	glutil::MatrixStack modelview = Canvas::instance().modelview();

	// Set light and materials in main shader program
  	glm::vec4 light_position(0, 100, 0, 1);
  	glm::vec4 light_eye = modelview.top() * light_position;

	Lighting::setPosition(light_eye);
	Lighting::setReflectance(glm::vec3(1.0f, 0.443f, 0.654f), glm::vec3(1.0f, 0.356f, 0.603f), glm::vec3(0.1f), 15.0f);

	modelview.push();
	main->setUniform("matrices.modelview", modelview.top());
	mesh_->render();
	modelview.pop();

	for (unsigned int i = 0; i < supports_.size(); i++) {
		supports_.at(i)->render();
	}
}

Spline* Track::spline()
{
	return spline_;
}