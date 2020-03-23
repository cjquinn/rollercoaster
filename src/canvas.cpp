#include "canvas.h"

#include "common.h"

// setup includes
#include "fbo.h"
#include "timer.h"
#include "window.h"

// Canvas includes
#include "billboard.h"
#include "camera.h"
#include "cart.h"
#include "frame.h"
#include "lighting.h"
#include "matrixstack.h"
#include "penguins.h"
#include "shader.h"
#include "shaderprogram.h"
#include "skybox.h"
#include "splinegun.h"
#include "terrain.h"
#include "track.h"

static void processEvents(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action != GLFW_RELEASE) {
    return;
  }

  Canvas &canvas = Canvas::instance();

  switch (key) {
    case GLFW_KEY_1:
      canvas.camera()->setState(FREE_VIEW);
      break;

    case GLFW_KEY_2:
      canvas.camera()->setState(FIRST_PERSON);
      break;

    case GLFW_KEY_3:
      canvas.camera()->setState(SIDE_VIEW);
      break;

    case GLFW_KEY_4:
      canvas.camera()->setState(TOP_VIEW);
      break;

    case GLFW_KEY_5:
      canvas.camera()->setState(BILLBOARD);
      break;

    case GLFW_KEY_L:
      canvas.setIntensity(canvas.intensity() == 1.0f ? 0.1f : 1.0f);
      canvas.setSpotlightIntensity(canvas.spotlightIntensity() == 0.0f ? 0.5f : 0.0f);
      canvas.penguins()->setNight();
      break;
  }
}

Canvas::Canvas() :
	//Canvas Objects
  intensity_(1.0f), spotlight_intensity_(0.0f),
	billboard_(NULL), camera_(NULL), cart_(NULL), penguins_(NULL), skybox_(NULL), terrain_(NULL), track_(NULL),
  dt_(0.0), fbo_(NULL), fps_(0), timer_(NULL), window_ (Window::instance())
{
}

Canvas::~Canvas() 
{ 
  // Canvas objects
	delete billboard_;
  delete camera_;
	delete cart_;
	delete penguins_;
  delete skybox_;
	delete terrain_;
	delete track_;

  while(!shader_programs_.empty()) {
    delete shader_programs_.back();
    shader_programs_.pop_back();
  }

  // setup objects
	delete fbo_;
  delete timer_;
}

int Canvas::exec() 
{
  timer_ = new Timer;

  if(!window_.init(800, 600, (char*) "Rollercoaster")) {
    return -1;
  }

  glfwSetKeyCallback(Window::instance().glwfWindow(), processEvents);

  init();

  timer_->start();
  double frame_duration = 1000.0 / (double) Canvas::FPS;

  while (!glfwWindowShouldClose(window_.glwfWindow())) {
    timer_->start();

    update();
    render();

    dt_ = timer_->elapsed();

    glfwPollEvents();
  }

  glfwDestroyWindow(window_.glwfWindow());
  glfwTerminate();

  return 0;
}
void Canvas::init() 
{
  int windowWidth, windowHeight;
  glfwGetFramebufferSize(Window::instance().glwfWindow(), &windowWidth, &windowHeight);

  // Set the clear colour and depth
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClearDepth(1.0f);

	// Utitlity creates
	fbo_ = new FBO;
	fbo_->create(windowWidth, windowHeight);
	fbo_->addDepthBuffer();

  // Create objects
	billboard_ = new Billboard;
  camera_ = new Camera;
	cart_ = new Cart;
	penguins_ = new Penguins;
	skybox_ = new Skybox;
	terrain_ = new Terrain;
	track_ = new Track;

  // Set the orthographic and perspective projection matrices based on the image size
  camera_->setOrthographic(windowWidth, windowHeight); 
  camera_->setPerspective(45.0f, (float) windowWidth / (float) windowHeight, 0.5f, 5000.0f);

  // Load shaders
  std::vector<Shader> shaders;
  std::vector<std::string> shader_filenames;

  shader_filenames.push_back("main.vert");
	shader_filenames.push_back("main.frag");

  for (unsigned int i = 0; i < shader_filenames.size(); ++i) {
    std::string ext = shader_filenames[i].substr((int) shader_filenames[i].size() - 4, 4);
    int shader_type = ext == "vert" ? GL_VERTEX_SHADER : (ext == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER);

    Shader shader;
    shader.loadShader(ROOT_DIR + std::string("/resources/shaders/") + shader_filenames[i], shader_type);
    shaders.push_back(shader);
  }

  // Create the main shader program
  ShaderProgram *main = new ShaderProgram;
  main->create();
  main->addShader(&shaders[0]);
  main->addShader(&shaders[1]);
  main->link();
	main->use();
	main->setUniform("sampler", 0);
  shader_programs_.push_back(main);

	// Canvas creates
  skybox_->create(ROOT_DIR + std::string("/resources/skyboxes/toon_snow/"), "front.jpg", "back.jpg", "left.jpg", "right.jpg", "top.jpg", 2048.0f);
	terrain_->create(ROOT_DIR + std::string("/resources/heightmap/heightmap.bmp"), glm::vec3(0.0f), 1024.0f, 1024.0f, 100.0f);

	// Terrain dependent creates
	billboard_->create(glm::vec3(350.0f, 0.0f, 130.0f));
	penguins_->create(20, glm::vec3(-200.0f, 0.0f, -200.0f));
	penguins_->create(20, glm::vec3(200.0f, 0.0f, 250.0f));

	// Rollercoaster creates
	track_->create(ROOT_DIR + std::string("/resources/track/main.csv"));
	cart_->create(track_->spline());
}

void Canvas::render() 
{  
	fbo_->bind();
  // Clear the buffers and enable depth testing (z-buffering)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

	modelview_.setIdentity();
	modelview_.lookAt(cart_->frame()->p() + 5.0f * cart_->frame()->b() - cart_->frame()->t() * 0.5f, cart_->frame()->p() + 30.0f * cart_->frame()->t(), camera_->up_vector());

	ShaderProgram *main = shader_programs(0);

	main->use();
	main->setUniform("matrices.projection", camera_->perspective());
	main->setUniform("matrices.normal", camera_->normal(modelview_.top()));
	
	Lighting::setIntensity(intensity_);
	Lighting::setSpotlightIntensity(0.0f);

	// // Canvas renders
	penguins_->render();
	skybox_->render();
	terrain_->render();

	// Lighting::setSpotlightIntensity(spotlight_intensity_);
	glm::vec3 spotlight_position = cart_->frame()->p() + cart_->frame()->b() * 2.0f;
	glm::vec4 light_eye = modelview_.top() * glm::vec4(spotlight_position, 1.0f);
	Lighting::setSpotlight(light_eye, glm::normalize(camera_->normal(modelview_.top()) * cart_->frame()->t()), 0.0f, 5.0f); 
	
	// // Rollercoaster renders
	track_->render();
	cart_->render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear the buffers and enable depth testing (z-buffering)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

	modelview_.setIdentity();
	modelview_.lookAt(camera_->position(), camera_->view(), camera_->up_vector());

	main->use();
	main->setUniform("matrices.projection", camera_->perspective());
	main->setUniform("matrices.normal", camera_->normal(modelview_.top()));
	
	Lighting::setIntensity(intensity_);
	Lighting::setSpotlightIntensity(0.0f);

	// Canvas renders
	fbo_->bindTexture(0, true);
	
	Lighting::setIntensity(1.0f);
	billboard_->render();	
	Lighting::setIntensity(intensity_);

	penguins_->render();
  terrain_->render();
	skybox_->render();

	Lighting::setSpotlightIntensity(spotlight_intensity_);
	Lighting::setSpotlight(light_eye, glm::normalize(camera_->normal(modelview_.top()) * cart_->frame()->t()), 0.0f, 5.0f); 
	
	// Rollercoaster renders
	track_->render();
	cart_->render();

  // Swap buffers to show the rendered image
  glfwSwapBuffers(window_.glwfWindow());
}

void Canvas::update() 
{
  camera_->update(dt_);
	cart_->update(dt_);
}

Canvas& Canvas::instance() 
{
  static Canvas instance;

  return instance;
}

// Canvas object getters
Camera* Canvas::camera() 
{
	return camera_;
}

Cart* Canvas::cart()
{
	return cart_;
}

glutil::MatrixStack Canvas::modelview() 
{
	return modelview_;
}

Penguins* Canvas::penguins()
{
	return penguins_;
}

ShaderProgram* Canvas::shader_programs(int i) 
{
	return shader_programs_[i];
}

Terrain* Canvas::terrain()
{
	return terrain_;
}

float Canvas::intensity() {
  return intensity_;
}

void Canvas::setIntensity(float intensity) {
  intensity_ = intensity;
}

float Canvas::spotlightIntensity() {
  return spotlight_intensity_;
}

void Canvas::setSpotlightIntensity(float spotlightIntensity) {
  spotlight_intensity_ = spotlightIntensity;
}
