#ifndef MESH_H
#define MESH_H

#include "common.h"
#include "texture.h"
#include "vbo.h"

class Vertex;

typedef struct {
  std::vector<unsigned int> id;
} TriangleList;


class Mesh
{
public:
	Mesh();
	~Mesh();

	void create(const Texture &texture, const std::vector<Vertex> &verticies, const std::vector<unsigned int> &triangles);
	
	void computeVertexNormals();
	glm::vec3 computeTriangleNormal(unsigned int id);

	void computeTextureCoords();
	
	void render();
	void release();

private:
	UINT vao_;
  VBO vbo_;
  Texture texture_;

	std::vector<Vertex> vertices_;
  std::vector<unsigned int> triangles_;
  std::vector<TriangleList> on_triangle_;
};

#endif