#include "support.h"

#include "canvas.h"
#include "circle.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "mesh.h"
#include "shaderprogram.h"
#include "terrain.h"
#include "vertex.h"

Support::Support() : mesh_(NULL)
{}

Support::~Support()
{
	delete mesh_;
}

void Support::create(glm::vec3 p)
{
	mesh_ = new Mesh;

	std::vector<Circle*> circles;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	int samples = 4;

	glm::vec3 f(p.x, Canvas::instance().terrain()->groundHeight(p), p.z);

	Circle *start, *end;

	start = new Circle;
	end = new Circle;

	start->create(p, samples, 1);
	end->create(f, samples, 1);

	circles.push_back(start);
	circles.push_back(end);

	for (int i = 0; i < samples; ++i) {
		for (unsigned int j = 0; j < circles.size(); ++j) {
			Vertex v(circles.at(j)->vertices().at(i), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
      vertices.push_back(v);
		}
	}

	for (int i = 0; i < samples; ++i) {
		for (unsigned int j = 0; j < circles.size(); ++j) {
      int index = j + i * circles.size();

      triangles.push_back(index % vertices.size());
      triangles.push_back((index + 1 + circles.size()) % vertices.size());
     	triangles.push_back((index + 1) % vertices.size()); 

			triangles.push_back(index % vertices.size());
      triangles.push_back((index + circles.size()) % vertices.size());
			triangles.push_back((index + 1 + circles.size()) % vertices.size());
    }
  }

	mesh_->create(vertices, triangles, false);
}

void Support::render()
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

	Lighting::setPosition(light_eye);
	Lighting::setReflectance(glm::vec3(0.0f, 0.443f, 0.654f), glm::vec3(0.0f, 0.356f, 0.603f), glm::vec3(0.1f), 15.0f);

	modelview.push();
	  main->setUniform("matrices.modelview", modelview.top());
		mesh_->render();
	modelview.pop();
}