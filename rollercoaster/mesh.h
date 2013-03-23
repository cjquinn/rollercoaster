#ifndef MESH_H
#define MESH_H

#include "common.h"
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

	void create(const std::vector<Vertex> &verticies, const std::vector<unsigned int> &triangles, bool texture);
	
	void computeVertexNormals();
	glm::vec3 computeTriangleNormal(unsigned int id);

	void computeTextureCoords();
	
	void render();

private:
	UINT vao_;
  VBO vbo_;

	std::vector<Vertex> vertices_;
  std::vector<unsigned int> triangles_;
  std::vector<TriangleList> on_triangle_;
};

#endif