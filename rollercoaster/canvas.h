#ifndef CANVAS_H
#define CANVAS_H

#include "common.h"
#include "matrixstack.h"

// Setup classes
class Window;

// Utility classes
class FBO;
class Shader;
class ShaderProgram;
class Timer;

// Canvas classes
class Billboard;
class Camera;
class Cart;
class Penguins;
class Skybox;
class Terrain;
class Track;

class Canvas 
{
public:
	Camera *camera();
	Cart* cart();
	glutil::MatrixStack modelview();
	ShaderProgram* shader_programs(int i = 0);
	Terrain* terrain();

private:
	void init();
  void update();
  void render();

	float intensity_;
	float spotlight_intensity_;

	glutil::MatrixStack modelview_;
	std::vector<ShaderProgram *> shader_programs_;

	Billboard *billboard_;
  Camera *camera_;
	Cart *cart_;
	Penguins *penguins_;
	Skybox *skybox_;
	Terrain *terrain_;
	Track *track_;

public:
  enum {
    FPS = 30
  };

  static Canvas& instance();
  ~Canvas();

  WPARAM exec();

  void setHInstance(HINSTANCE hinstance);
  LRESULT processEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);

private:
  Canvas::Canvas();
  Canvas::Canvas(const Canvas&);
  void Canvas::operator=(const Canvas&);

  void renderFPS();
	void renderLightingVariables();

  Window &window_;
  HINSTANCE hinstance_;

  double dt_;
  int fps_;
  bool active_;

	FBO *fbo_;
	Timer *timer_;
};

#endif
