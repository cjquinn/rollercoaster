#include "support.h"

#include "canvas.h"
#include "circle.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "shaderprogram.h"
#include "terrain.h"
#include "vertex.h"

Support::Support()
{}

Support::~Support()
{}

void Support::create(glm::vec3 p)
{
	std::vector<Circle*> circles;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	unsigned int samples = 4;

	glm::vec3 end_position(p.x, Canvas::instance().terrain()->getTerrainHeight(p), p.z);
	glm::vec3 up_vector(0.0f, 0.0f, 1.0f);

	Frame *start_frame = new Frame(p, end_position, up_vector);
	Frame *end_frame = new Frame(end_position, end_position - end_position, up_vector);

	Circle *start, *end;

	start = new Circle;
	end = new Circle;

	start->create(start_frame, samples, 1);
	end->create(end_frame, samples, 1);

	circles.push_back(start);
	circles.push_back(end);

	for (unsigned int i = 0; i < samples; ++i) {
		for (unsigned int j = 0; j < circles.size(); ++j) {
			Vertex v(circles.at(j)->vertices().at(i), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
      vertices.push_back(v);
		}
	}

	for (int z = 0; z < samples - 1; ++z) {
    for (int x = 0; x < circles.size() - 1; ++x) {
      int index = x + z * circles.size();

      triangles.push_back(index);
      triangles.push_back(index + 1 + circles.size());
      triangles.push_back(index + 1);

      triangles.push_back(index);
      triangles.push_back(index + circles.size());
      triangles.push_back(index + 1 + circles.size());
    }
  }

	mesh_.create(vertices, triangles);
}

void Support::render()
{
		ShaderProgram *main = Canvas::instance().shader_programs();
	main->use();
	main->setUniform("toonify", false);
	main->setUniform("texture_fragment", false);

	// Set up a matrix stack
  glutil::MatrixStack modelview = Canvas::instance().modelview();

	// Set light and materials in main shader program
  glm::vec4 light_position(0, 100, 0, 1);
  glm::vec4 light_eye = modelview.top() * light_position;

	Lighting::set(
		light_eye, 
		glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f),
		glm::vec3(0.0f, 0.443f, 0.654f), glm::vec3(0.0f, 0.356f, 0.603f), glm::vec3(0.1f),
		15.0f);

	modelview.push();
	  main->setUniform("matrices.modelview", modelview.top());
		mesh_.render();
	modelview.pop();
}