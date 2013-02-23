#include "mesh.h"

#include "vertex.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

glm::vec3 Mesh::computeTriangleNormal(unsigned int id)
{
  Vertex v0, v1, v2;
  glm::vec3 normal, q;

  v0 = vertices_[triangles_[3 * id]];
  v1 = vertices_[triangles_[3 * id + 1]];
  v2 = vertices_[triangles_[3 * id + 2]];

	glm::vec3 const & a = v0.position;
  glm::vec3 const & b = v1.position;
  glm::vec3 const & c = v2.position;

  return glm::normalize(glm::cross(c - a, b - a));
}


void Mesh::computeTextureCoords()
{
  for (unsigned int i = 0; i < vertices_.size(); ++i) {
    vertices_[i].texture.x = vertices_[i].position.x / 5.0f;
    vertices_[i].texture.y = vertices_[i].position.z / 5.0f;
  }
}

void Mesh::computeVertexNormals()
{
  for (unsigned int i = 0; i < vertices_.size(); ++i) {
    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
    int faces = 0;
    for (unsigned int j = 0; j < on_triangle_[i].id.size(); ++j) {
      int triangle = on_triangle_[i].id[j];
      normal += computeTriangleNormal(triangle);
      ++faces;
    }
    normal = normal / (float) faces;
    glm::normalize(normal);

    vertices_[i].normal.x = normal.x;
    vertices_[i].normal.y = normal.y;
    vertices_[i].normal.z = normal.z;
  }
}



void Mesh::create(const Texture &texture, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &triangles)
{
	texture_ = texture;
  vertices_ = vertices;
  triangles_ = triangles;

  on_triangle_.resize(vertices_.size());
  unsigned int count = (unsigned int) (triangles_.size() / 3);
  for (unsigned int i = 0; i < count; ++i) {
    on_triangle_[triangles_[i * 3]].id.push_back(i);
    on_triangle_[triangles_[i * 3 + 1]].id.push_back(i);
    on_triangle_[triangles_[i * 3 + 2]].id.push_back(i);
  }

  computeVertexNormals();
  computeTextureCoords();

	// Use VAO to store state associated with vertices
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // Create a VBO
  vbo_.create();
  vbo_.bind();

  // A colour
  //glm::vec3 colour = glm::vec3(0.0f, 1.0f, 0.0f);

  // Put the vertex attributes in the VBO
  for (unsigned int i = 0; i < vertices_.size(); ++i) {
    vbo_.addData(&vertices_[i].position, sizeof(glm::vec3));
		vbo_.addData(&vertices_[i].texture, sizeof(glm::vec2));
		vbo_.addData(&vertices_[i].normal, sizeof(glm::vec3));
  }
	
  // Upload the VBO to the GPU
  vbo_.uploadDataToGPU(GL_STATIC_DRAW);

  // Set the vertex attribute locations
  GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

  // Texture coordinates
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) sizeof(glm::vec3));

  // Normal vectors
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (sizeof(glm::vec3) + sizeof(glm::vec2)));
}

// Render the plane as a triangle strip
void Mesh::render()
{
  glBindVertexArray(vao_);
  texture_.bind();
  glDrawElements(GL_TRIANGLES, triangles_.size(), GL_UNSIGNED_INT, &triangles_[0]);
}

// Release resources
void Mesh::release()
{
  texture_.release();
  glDeleteVertexArrays(1, &vao_);
  vbo_.release();
}
