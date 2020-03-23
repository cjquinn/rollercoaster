#ifndef PLANE_H
#define PLANE_H

#include "common.h"

#include "texture.h"
#include "vbo.h"

// Class for generating a xz plane of a given size
class Plane
{
public:
  void create(std::string directory, std::string filename, float width, float height, float texture_repeat);
  void render();
  void release();

private:
  GLuint vao_;
  VBO vbo_;
  Texture texture_;
  std::string directory_;
  std::string filename_;
  float width_;
  float height_;
};

#endif
