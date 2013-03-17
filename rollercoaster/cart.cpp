#include "cart.h"

#include "canvas.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "objmodel.h"
#include "shaderprogram.h"
#include "spline.h"

Cart::Cart() : frame_(NULL), model_(NULL), spline_(NULL)
{}

Cart::~Cart()
{
	model_->release();
	delete model_;
}

void Cart::create(Spline *spline)
{
	model_ = new ObjModel;
	model_->load("resources\\models\\cart\\cart.obj", "cart.mtl"); 

	spline_ = spline;
}

void Cart::render()
{
	if(spline_) {
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
			glm::vec3(0.352f, 0.443f, 0.654f), glm::vec3(0.247f, 0.356f, 0.603f), glm::vec3(0.1f),
			15.0f);

		modelview.push();
			modelview.translate(frame_->p() + 2.0f * frame_->b());
			glm::mat4 r = glm::mat4(glm::mat3(frame_->t(), frame_->b(), frame_->n()));
			modelview *= r;
			main->setUniform("matrices.modelview", modelview.top());
			model_->render();
		modelview.pop();
	}
}

void Cart::update(float dt)
{
	if(spline_) {
		static float t = 0.0f;
		t += 0.005f * (float) dt;

		glm::vec3 p = spline_->pointAt(t);
		glm::vec3 q = spline_->pointAt(t += 0.05f * (float) dt);

		frame_ = new Frame(p, q);
	}
}