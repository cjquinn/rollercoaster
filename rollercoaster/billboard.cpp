#include "billboard.h"

#include "canvas.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "objmodel.h"
#include "quad.h"
#include "shaderprogram.h"
#include "terrain.h"

Billboard::Billboard() : frame_(NULL), model_(NULL), poster_(NULL)
{}

Billboard::~Billboard()
{
	delete frame_;
	delete model_;
	delete poster_;
}

void Billboard::create(glm::vec3 p)
{
	frame_ = new Frame(p, p + glm::vec3(0.0f, 0.0f, -5.0f));
	model_ = new ObjModel;
	poster_ = new Quad;

	model_->load("resources\\models\\billboard\\billboard.obj", "billboard.mtl");
	poster_->create(14.0f, 9.0f);
}

void Billboard::render()
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

	Lighting::setPosition(light_eye);
	Lighting::setReflectance(glm::vec3(0.352f, 0.443f, 0.654f), glm::vec3(0.247f, 0.356f, 0.603f), glm::vec3(0.1f), 15.0f);

	float y = Canvas::instance().terrain()->groundHeight(frame_->p());

	modelview.push();
		modelview.translate(glm::vec3(frame_->p().x, y, frame_->p().z));
		modelview.scale(5.0f);
		main->setUniform("matrices.modelview", modelview.top());
		model_->render();
	modelview.pop();

	main->setUniform("toonify", false);
	main->setUniform("texture_fragment", false);

	Lighting::white();

	modelview.push();
		modelview.translate(glm::vec3(frame_->p().x - 0.25f, y + 23.1f, frame_->p().z + 1.9f));
		glm::mat4 r = glm::mat4(glm::mat3(frame_->n(), frame_->t(), frame_->b()));
		modelview *= r;
		main->setUniform("matrices.modelview", modelview.top());
		poster_->render();
	modelview.pop();
}