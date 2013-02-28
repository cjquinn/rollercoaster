#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

#include "mesh.h"

class Terrain
{
public:
	Terrain();
	~Terrain();

  bool create(char *filename, float size_x, float size_z, float scale);
  float getTerrainHeight(glm::vec3 p);

	void render();
private:
	glm::vec3 worldToImage(glm::vec3 p);
	glm::vec3 imageToWorld(glm::vec3 p);

	float *heightmap_;

	int width_;
	int height_;
	float size_x_;
	float size_z_;

	Mesh mesh_;
};

#endif