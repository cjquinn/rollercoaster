#ifndef CANVAS_H
#define CANVAS_H

#include "common.h"

#include "matrixstack.h"

// Setup classes
class Window;

// Utility classes
class FreeTypeFont;
class Shader;
class ShaderProgram;
class Timer;

// Canvas classes
class Camera;
class Skybox;
class Terrain;

class Canvas 
{
public:

private:
	void init();
  void update();
  void render();

	std::vector<ShaderProgram *> shader_programs_;
	glutil::MatrixStack modelview_;

  Camera *camera_;
	Skybox *skybox_;
	Terrain *terrain_;

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

  FreeTypeFont *font_;
	Timer *timer_;
};

#endif
