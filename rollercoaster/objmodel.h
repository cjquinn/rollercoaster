#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "common.h"
#include "vbo.h"

class Texture;

// Class for handling obj files
class ObjModel
{
public:
  ObjModel();
	~ObjModel();

  bool load(std::string file, std::string material);
  void render();

  int polygonCount();

private:
  bool loaded_;
  int attributes_;
  int faces_;
  VBO vbo_;
  UINT vao_;
  Texture *texture_;

  bool loadMaterial(std::string filename);

};

#endif
