#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

struct FIBITMAP;
class Mesh;
class Texture;

class Terrain
{
public:
	Terrain();
	~Terrain();

	void create(char *heightmap, glm::vec3 origin, float size_x, float size_z, float scale);
	float groundHeight(glm::vec3 p);
	
	void render();

private:
	glm::vec3 worldToImage(glm::vec3 p);
	glm::vec3 imageToWorld(glm::vec3 p);

	int width_, height_;
	float *heightmap_;
	float size_x_, size_z_;
	
	glm::vec3 origin_;

	FIBITMAP *dib_;
	Mesh *mesh_;
	Texture *texture_;
};

#endif