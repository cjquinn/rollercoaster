#include "track.h"

#include "canvas.h"
#include "circle.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "shaderprogram.h"
#include "support.h"
#include "vertex.h"

Track::Track()
{}

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
}

void Track::create(Spline *spline) 
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	unsigned int samples = 5;

	for (unsigned int i = 0; i < spline->sampled_points().size() - 1; i++) {
		Circle *circle = new Circle;
		circle->create(new Frame(spline->sampled_points().at(i), spline->sampled_points().at(i+1), glm::vec3(0.0f, 1.0f, 0.0f)), (int) samples, 5.0f);
		circles_.push_back(circle);

		/*if(i % 20 == 0) {
			Support *support = new Support;
			support->create(spline->sampled_points().at(i));
			supports_.push_back(support);
		}*/
	}

	for (unsigned int i = 0; i < samples; ++i) {
		for (unsigned int j = 0; j < circles_.size(); ++j) {
			Vertex v(circles_.at(j)->vertices().at(i), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
      vertices.push_back(v);
		}
	}

	for (unsigned int z = 0; z < samples; ++z) {
    for (unsigned int x = 0; x < circles_.size(); ++x) {
      int index = x + z * circles_.size();

			triangles.push_back((index + 1) % vertices.size());
      triangles.push_back((index + 1 + circles_.size()) % vertices.size());
      triangles.push_back(index % vertices.size());
      
			triangles.push_back((index + 1 + circles_.size()) % vertices.size());
      triangles.push_back((index + circles_.size()) % vertices.size());
      triangles.push_back(index % vertices.size());
      
		}
  }

	mesh_.create(vertices, triangles);
}

void Track::render()
{
	ShaderProgram *main = Canvas::instance().shader_programs();
	main->use();
	main->setUniform("toonify", true);
	main->setUniform("texture_fragment", false);

	// Set up a matrix stack
  glutil::MatrixStack modelview = Canvas::instance().modelview();

	// Set light and materials in main shader program
  glm::vec4 light_position(0, 100, 0, 1);
  glm::vec4 light_eye = modelview.top() * light_position;

	Lighting::set(
		light_eye, 
		glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f),
		glm::vec3(1.0f, 0.443f, 0.654f), glm::vec3(1.0f, 0.356f, 0.603f), glm::vec3(0.1f),
		15.0f);

	modelview.push();
	  main->setUniform("matrices.modelview", modelview.top());
		mesh_.render();
	modelview.pop();

	for (unsigned int i = 0; i < supports_.size(); i++) {
		supports_.at(i)->render();
	}
}