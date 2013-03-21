#include "quad.h"

#include "canvas.h"
#include "mesh.h"
#include "texture.h"
#include "vertex.h"

Quad::Quad() : mesh_(NULL), texture_(NULL)
{}

Quad::~Quad()
{
	delete texture_;
	delete mesh_;
}

void Quad::create(float w, float h)
{
	mesh_ = new Mesh;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;
	
	Vertex v0(glm::vec3(-w, 0.0f, -h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	Vertex v1(glm::vec3(-w, 0.0f, h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	Vertex v2(glm::vec3(w, 0.0f, h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	Vertex v3(glm::vec3(w, 0.0f, -h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	
	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	triangles.push_back(0);
	triangles.push_back(2);
	triangles.push_back(3);

	triangles.push_back(0);
	triangles.push_back(1);
	triangles.push_back(2);

	mesh_->create(vertices, triangles);
}

void Quad::create(float w, float h, std::string texture)
{}

void Quad::render()
{
	mesh_->render();
}