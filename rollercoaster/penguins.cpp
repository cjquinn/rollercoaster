#include "penguins.h"

#include "canvas.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "objmodel.h"
#include "shaderprogram.h"
#include "terrain.h"

Penguins::Penguins() : model_(NULL)
{}

Penguins::~Penguins()
{
	model_->release();
	delete model_;
}

void Penguins::create(int n, glm::vec3 origin)
{
	model_ = new ObjModel;
	model_->load("resources\\models\\penguin\\penguin.obj", "penguin.mtl");

	for (int i = 0; i < n/2; i++) {
		for (int j = 0; j < n/2; j++) { 
			glm::vec3 p(rand() % 100 + j, 0.0f,rand() % 100 + i);
			p += origin;
			float y = Canvas::instance().terrain()->groundHeight(p);
			positions_.push_back(glm::vec3(p.x, y, p.z));
			Frame frame(positions_.at(i), glm::vec3(positions_.at(i*j).x + (rand() % 10 + 1) * 0.1, positions_.at(i*j).y, positions_.at(i*j).z + (rand() % 10 + 1) * 0.1));
			rotations_.push_back(glm::mat4(glm::mat3(frame.t(), frame.b(), frame.n())));
			//fix this
			scales_.push_back((rand() % 10 + 5) * 0.1f);
		}
	}
}

void Penguins::render()
{
	ShaderProgram *main = Canvas::instance().shader_programs();
	main->use();
	main->setUniform("toonify", true);
	main->setUniform("texture_fragment", true);

	// Set up a matrix stack
	glutil::MatrixStack modelview = Canvas::instance().modelview();

	// Set light and materials in main shader program
	glm::vec4 light_position(0, 100, 0, 1);
	glm::vec4 light_eye = modelview.top() * light_position;

	Lighting::set(
		light_eye, 
		glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f),
		glm::vec3(0.247f, 0.356f, 0.603f), glm::vec3(1.0f), glm::vec3(0.1f),
		15.0f);

	for(unsigned int i = 0; i < positions_.size(); i++) {
		modelview.push();
			modelview.translate(positions_.at(i));
			modelview *= rotations_.at(i);
			modelview.scale(scales_.at(i));
			main->setUniform("matrices.modelview", modelview.top());
			model_->render();
		modelview.pop();
	}
}