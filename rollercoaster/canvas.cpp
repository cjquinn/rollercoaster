#include "canvas.h"

#include "include/gl/glew.h"
#include <gl/gl.h>

// setup includes
#include "timer.h"
#include "window.h"

// Canvas includes
#include "camera.h"
#include "freetypefont.h"
#include "matrixstack.h"
#include "shader.h"
#include "shaderprogram.h"
#include "skybox.h"
#include "terrain.h"

Canvas::Canvas() :
	//Canvas Objects
  camera_(NULL), font_(NULL), skybox_(NULL), terrain_(NULL),  
  dt_(0.0), fps_(0), timer_(NULL), window_ (Window::instance())
{
}

Canvas::~Canvas() 
{ 
  // Canvas objects
  delete camera_;
  delete font_;
  delete skybox_;
	delete terrain_;

  while(!shader_programs_.empty()) {
    delete shader_programs_.back();
    shader_programs_.pop_back();
  }

  // setup objects
  delete timer_;
}

void Canvas::init() 
{
  // Set the clear colour and depth
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClearDepth(1.0f);

  /// Create objects
  camera_ = new Camera;
  font_ = new FreeTypeFont;
	terrain_ = new Terrain;
  skybox_ = new Skybox;

  RECT dimensions = window_.dimensions();

  int width = dimensions.right - dimensions.left;
  int height = dimensions.bottom - dimensions.top;

  // Set the orthographic and perspective projection matrices based on the image size
  camera_->setOrthographicMatrix(width, height); 
  camera_->setPerspectiveMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

  // Load shaders
  std::vector<Shader> shaders;
  std::vector<std::string> shader_filenames;

  shader_filenames.push_back("perVertexLighting.vert");
  shader_filenames.push_back("perVertexLighting.frag");
  shader_filenames.push_back("ortho2D.vert");
  shader_filenames.push_back("font2D.frag");

  for (int i = 0; i < (int) shader_filenames.size(); ++i) {
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
  shader_programs_.push_back(main);
  
  // Create a shader program for fonts
  ShaderProgram *fonts = new ShaderProgram;
  fonts->create();
  fonts->addShader(&shaders[2]);
  fonts->addShader(&shaders[3]);
  fonts->link();
  shader_programs_.push_back(fonts);

	font_->loadSystemFont("arial.ttf", 32);
	font_->setShaderProgram(fonts);
	
  // Create the skybox
  // Skybox downloaded from http://www.redsorceress.com/skybox.html
  skybox_->create("resources\\skyboxes\\toon_snow\\", "front.jpg", "back.jpg", "left.jpg", "right.jpg", "top.jpg", 2048.0f);

	//Create the terrain
  terrain_->create("resources\\heightmap\\heightmap.bmp", 2048.0f, 2048.0f, 40.0f);
}

void Canvas::render() 
{  
  // Clear the buffers and enable depth testing (z-buffering)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // Set up a matrix stack
  glutil::MatrixStack modelview;
  modelview.setIdentity();

  // Use the main shader program 
  ShaderProgram *main = (shader_programs_)[0];
  main->use();
  main->setUniform("bUseTexture", true);
	// Set the texture sampler in the fragment shader
  main->setUniform("gSampler", 0);

  // Set the projection and modelview matrix based on the current camera location  
  main->setUniform("matrices.projMatrix", camera_->perspectiveMatrix());
  modelview.lookAt(camera_->position(), camera_->view(), camera_->upVector());

  // Set light and materials in main shader program
  glm::vec4 light_position(1000, 0, 1000, 1);
  glm::vec4 light_eye = modelview.top() * light_position;

  main->setUniform("light1.position", light_eye); // Position of light source in eye coordinates
  main->setUniform("light1.La", glm::vec3(1.0f)); // Ambient colour of light
  main->setUniform("light1.Ld", glm::vec3(1.0f)); // Diffuse colour of light
  main->setUniform("light1.Ls", glm::vec3(1.0f)); // Specular colour of light
  main->setUniform("material1.Ma", glm::vec3(1.0f)); // Ambient material reflectance
  main->setUniform("material1.Md", glm::vec3(0.0f)); // Diffuse material reflectance
  main->setUniform("material1.Ms", glm::vec3(0.0f)); // Specular material reflectance
  main->setUniform("material1.shininess", 15.0f); // Shininess material property

  // Render the skybox and terrain with full ambient light 
  modelview.push();
    // Translate the modelview matrix to the camera eye point so skybox stays centred around camera
    glm::vec3 eye = camera_->position();
    modelview.translate(eye);

    main->setUniform("matrices.modelViewMatrix", modelview.top());
    main->setUniform("matrices.normalMatrix", camera_->normalMatrix(modelview.top()));

    skybox_->render();
  modelview.pop();

	modelview.push();
	  main->setUniform("matrices.modelViewMatrix", modelview.top());
    main->setUniform("matrices.normalMatrix", camera_->normalMatrix(modelview.top()));
		//main->setUniform("bUseTexture", false);
    terrain_->render();
	modelview.pop();

  renderFPS();

  // Swap buffers to show the rendered image
  SwapBuffers(window_.hdc());    
}



void Canvas::update() 
{
  // Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
  camera_->update(dt_);
}



void Canvas::renderFPS()
{
  static int count = 0;
  static double elapsed = 0.0f;

  ShaderProgram *fonts = (shader_programs_)[1];

  RECT dimensions = window_.dimensions();
  int height = dimensions.bottom - dimensions.top;

  // Increase the elapsed time and frame counter
  elapsed += dt_;
  ++count;

  // Now we want to subtract the current time by the last time that was stored
  // to see if the time elapsed has been over a second, which means we found our FPS.
  if(elapsed > 1000 ) {
    elapsed = 0;
    fps_ = count;

    // Reset the frames per second
    count = 0;
  }

  if (fps_ > 0) {
    glDisable(GL_DEPTH_TEST);

    // Use the font shader program and render the text
    fonts->use();

    fonts->setUniform("matrices.modelViewMatrix", glm::mat4(1));
    fonts->setUniform("matrices.projMatrix", camera_->orthographicMatrix());
    fonts->setUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
		font_->printf(20, height - 20, 20, "FPS: %d", fps_);

    glEnable(GL_DEPTH_TEST);
  }
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