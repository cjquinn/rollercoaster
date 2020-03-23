#include "sphere.h"

Sphere::Sphere() {}

Sphere::~Sphere()
{
    glDeleteVertexArrays(1, &vao_);
    vbo_.release();
}

// Create a unit sphere
void Sphere::create(int slices, int stacks)
{
	glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    vbo_.create();
    vbo_.bind();

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    for (int i = 0; i < stacks; ++i) {
        float phi = (i / (float) stacks) * (float) M_PI;
        float next_phi;
        
        if (phi != stacks - 1) {
            next_phi = ((i + 1) / (float) stacks) * (float) M_PI;
        } else {
            next_phi = (float) M_PI;
        }

        for (int j = 0; j < slices; ++j) {
            float theta = (j / (float) slices) * 2 * (float) M_PI;
            float next_theta = (((j + 1) % slices) / (float) slices) * 2 * (float) M_PI;

            glm::vec3 v1 = glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
            glm::vec3 v2 = glm::vec3(cos(next_theta) * sin(phi), sin(next_theta) * sin(phi), cos(phi));
            glm::vec3 v3 = glm::vec3(cos(theta) * sin(next_phi), sin(theta) * sin(next_phi), cos(next_phi));
            glm::vec3 v4 = glm::vec3(cos(next_theta) * sin(next_phi), sin(next_theta) * sin(next_phi), cos(next_phi));

            vertices.push_back(v1);
            vertices.push_back(v4);
            vertices.push_back(v2);

            vertices.push_back(v1);
            vertices.push_back(v3);
            vertices.push_back(v4);

            normals.push_back(v1);
            normals.push_back(v4);
            normals.push_back(v2);

            normals.push_back(v1);
            normals.push_back(v3);
            normals.push_back(v4);
        }
    }


    for (int i = 0; i < (int) vertices.size(); ++i) {
        vbo_.addData(&vertices[i], sizeof(glm::vec3));
        vbo_.addData(&normals[i], sizeof(glm::vec3));
    }

    triangles_ = vertices.size() /  3;
    vbo_.uploadDataToGPU(GL_STATIC_DRAW);

    GLsizei stride = 2 * sizeof(glm::vec3);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *) (sizeof(glm::vec3) + sizeof(glm::vec2)));
}

// Render the sphere as a set of triangles
void Sphere::render()
{
	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLES, 0, triangles_ * 3);
}
