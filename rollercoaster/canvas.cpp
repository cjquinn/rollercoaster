#include "canvas.h"

#include "include/gl/glew.h"
#include <gl/gl.h>

// setup includes
#include "timer.h"
#include "window.h"

// Canvas includes
#include "camera.h"
#include "lighting.h"
#include "matrixstack.h"
#include "shader.h"
#include "shaderprogram.h"
#include "skybox.h"
#include "splinegun.h"
#include "terrain.h"

// Testing
#include "disk.h"

Canvas::Canvas() :
	//Canvas Objects
  camera_(NULL), skybox_(NULL), spline_gun_(NULL), terrain_(NULL),  
  dt_(0.0), fps_(0), timer_(NULL), window_ (Window::instance())
	// Testing
	, disk_(NULL)
{
}

Canvas::~Canvas() 
{ 
  // Canvas objects
  delete camera_;
  delete skybox_;
	delete spline_gun_;
	delete terrain_;

  while(!shader_programs_.empty()) {
    delete shader_programs_.back();
    shader_programs_.pop_back();
  }

  // setup objects
  delete timer_;

	// Testing
	delete disk_;
}

void Canvas::init() 
{
  // Set the clear colour and depth
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClearDepth(1.0f);

  /// Create objects
  camera_ = new Camera;
	spline_gun_ = new SplineGun;
	skybox_ = new Skybox;
	terrain_ = new Terrain;

	// Testing
	disk_ = new Disk;

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
	terrain_->create("resources\\heightmap\\heightmap.bmp", 2048.0f, 2048.0f, 40.0f);

	// Testing
	disk_->create("resources\\textures\\", "image.jpg", 8);
}

void Canvas::render() 
{  
  // Clear the buffers and enable depth testing (z-buffering)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

	Lighting::white();

	modelview_.setIdentity();
	modelview_.lookAt(camera_->position(), camera_->view(), camera_->up_vector());

	ShaderProgram *main = shader_programs();

	main->use();
	main->setUniform("matrices.projection", camera_->perspective());
	main->setUniform("matrices.normal", camera_->normal(modelview_.top()));
	
	// Canvas renders
	skybox_->render();
  terrain_->render();
	spline_gun_->render();

	main->use();
	main->setUniform("texture_fragment", false);
	main->setUniform("material.ambient", glm::vec3(1.0f, 0.0f, 0.0f));

	// Testing
	modelview_.push();
		modelview_.translate(25.0f, 1.0f, 25.0f);
		modelview_.scale(5.0f);

		main->setUniform("matrices.modelview", modelview_.top());

		disk_->render();
	modelview_.pop();

  // Swap buffers to show the rendered image
  SwapBuffers(window_.hdc());    
}

void Canvas::update() 
{
  camera_->update(dt_);
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
				case VK_SPACE:
					spline_gun_->addPoint(camera_->position());
					break;
				case VK_RETURN:
					camera_->follow(spline_gun_->spline());
					spline_gun_->setRender(false);
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
Camera *Canvas::camera() 
{
	return camera_;
}

glutil::MatrixStack Canvas::modelview() 
{
	return modelview_;
}

ShaderProgram *Canvas::shader_programs(int i) 
{
	return shader_programs_[i];
}