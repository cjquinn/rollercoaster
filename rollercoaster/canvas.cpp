#include "canvas.h"

#include "include/gl/glew.h"
#include <gl/gl.h>

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

void Canvas::init() 
{
  // Set the clear colour and depth
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClearDepth(1.0f);

	// Utitlity creates
	fbo_ = new FBO;
	fbo_->create(Window::instance().WIDTH, Window::instance().HEIGHT);
	fbo_->addDepthBuffer();

  /// Create objects
	billboard_ = new Billboard;
  camera_ = new Camera;
	cart_ = new Cart;
	penguins_ = new Penguins;
	skybox_ = new Skybox;
	terrain_ = new Terrain;
	track_ = new Track;

  RECT dimensions = window_.dimensions();
  int width = dimensions.right - dimensions.left;
  int height = dimensions.bottom - dimensions.top;

  // Set the orthographic and perspective projection matrices based on the image size
  camera_->setOrthographic(width, height); 
  camera_->setPerspective(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

  // Load shaders
  std::vector<Shader> shaders;
  std::vector<std::string> shader_filenames;

  shader_filenames.push_back("main.vert");
	shader_filenames.push_back("main.frag");

  for (unsigned int i = 0; i < shader_filenames.size(); ++i) {
    std::string ext = shader_filenames[i].substr((int) shader_filenames[i].size() - 4, 4);
    int shader_type = ext == "vert" ? GL_VERTEX_SHADER : (ext == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER);

    Shader shader;
    shader.loadShader("resources\\shaders\\" + shader_filenames[i], shader_type);
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
  skybox_->create("resources\\skyboxes\\toon_snow\\", "front.jpg", "back.jpg", "left.jpg", "right.jpg", "top.jpg", 2048.0f);
	terrain_->create("resources\\heightmap\\heightmap.bmp", glm::vec3(0.0f), 1024.0f, 1024.0f, 100.0f);

	// Terrain dependent creates
	billboard_->create(glm::vec3(350.0f, 0.0f, 130.0f));
	penguins_->create(20, glm::vec3(-200.0f, 0.0f, -200.0f));
	penguins_->create(20, glm::vec3(200.0f, 0.0f, 250.0f));

	// Rollercoaster creates
	track_->create("resources\\track\\main.csv");
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

	ShaderProgram *main = shader_programs();

	main->use();
	main->setUniform("matrices.projection", camera_->perspective());
	main->setUniform("matrices.normal", camera_->normal(modelview_.top()));
	
	Lighting::setIntensity(intensity_);
	Lighting::setSpotlightIntensity(0.0f);

	// Canvas renders
	penguins_->render();
	skybox_->render();
	terrain_->render();

	Lighting::setSpotlightIntensity(spotlight_intensity_);
	glm::vec3 spotlight_position = cart_->frame()->p() + cart_->frame()->b() * 2.0f;
	glm::vec4 light_eye = modelview_.top() * glm::vec4(spotlight_position, 1.0f);
	Lighting::setSpotlight(light_eye, glm::normalize(camera_->normal(modelview_.top()) * cart_->frame()->t()), 0.0f, 5.0f); 
	
	// Rollercoaster renders
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
  SwapBuffers(window_.hdc());    
}

void Canvas::update() 
{
  camera_->update(dt_);
	cart_->update(dt_);
}

WPARAM Canvas::exec() 
{
  timer_ = new Timer;
  window_.init(hinstance_);

  if(!window_.hdc()) {
    return 1;
  }

  init();

  timer_->start();
  double frame_duration = 1000.0 / (double) Canvas::FPS;

  MSG msg;
  while(true) {                          
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
      if(msg.message == WM_QUIT) {
        break;
      }

      TranslateMessage(&msg);  
      DispatchMessage(&msg);
    } else if (active_) {
      /*
      // Fixed timer
      dt_ = timer_->elapsed();

      if (dt_ > frame_duration) {
        timer_->start();

        update();
        render();
      }
      */

      // Variable timer
      timer_->start();

      update();
      render();

      dt_ = timer_->elapsed();
    } else {
      // Do not consume processor power if application isn't active -- Sam: really?!
      Sleep(200);
    }
  }

  window_.deinit();

  return msg.wParam;
}

LRESULT Canvas::processEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
  LRESULT result = 0;

  switch (message) {
    case WM_ACTIVATE:
      switch(LOWORD(w_param)) {
        case WA_ACTIVE:
        case WA_CLICKACTIVE:
          active_ = true;
          timer_->start();
        break;

        case WA_INACTIVE:
          active_ = false;
        break;
      }
    break;

    case WM_SIZE:
      RECT dimensions;
      GetClientRect(window, &dimensions);
      window_.setDimensions(dimensions);
    break;

    case WM_PAINT:
      PAINTSTRUCT ps;
      BeginPaint(window, &ps);
      EndPaint(window, &ps);
    break;

    case WM_KEYDOWN:
      switch(w_param) {
        case VK_ESCAPE:
          PostQuitMessage(0);
					break;
				// '1'
				case 0x31:
					camera_->setState(FIRST_PERSON);
					break;
				// '2'
				case 0x32:
					camera_->setState(SIDE_VIEW);
					break;
				// '3'
				case 0x33 :
					camera_->setState(TOP_VIEW);
					break;
				// '4'
				case 0x34 :
					camera_->setState(FREE_VIEW);
					break;
				// '5'
				case 0x35 :
					camera_->setState(BILLBOARD);
					break;
				// 'L'
				case 0x4C :
					intensity_ = (intensity_ == 1.0f) ? 0.1f : 1.0f;
					spotlight_intensity_ = (spotlight_intensity_ == 0.0f) ? 0.5f : 0.0f;
					penguins_->setNight();
					break;
      }
    break;

    case WM_DESTROY:
      PostQuitMessage(0);
    break;

    default:
      result = DefWindowProc(window, message, w_param, l_param);
    break;
  }

  return result;
}

Canvas& Canvas::instance() 
{
  static Canvas instance;

  return instance;
}

void Canvas::setHInstance(HINSTANCE hinstance) 
{
  hinstance_ = hinstance;
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

ShaderProgram* Canvas::shader_programs(int i) 
{
	return shader_programs_[i];
}

Terrain* Canvas::terrain()
{
	return terrain_;
}