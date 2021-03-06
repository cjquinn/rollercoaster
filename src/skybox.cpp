#include "skybox.h"

#include <iostream>

#include "canvas.h"
#include "camera.h"
#include "lighting.h"
#include "matrixstack.h"
#include "shaderprogram.h"
#include "texture.h"

Skybox::Skybox()
{}

Skybox::~Skybox()
{
	for (int i = 0; i < 5; ++i) {
        delete textures_[i];
    }

    glDeleteVertexArrays(1, &vao_);
    vbo_.release();
}

// Create a skybox of a given size with six textures
void Skybox::create(std::string directory, std::string front, std::string back, std::string left, std::string right, std::string top, /*std::string bottom,*/ float size)
{
	textures_[0] = new Texture;
	textures_[1] = new Texture;
	textures_[2] = new Texture;
	textures_[3] = new Texture;
	textures_[4] = new Texture;

    textures_[0]->load(directory + front);
    textures_[1]->load(directory + back);
    textures_[2]->load(directory + left);
    textures_[3]->load(directory + right);
    textures_[4]->load(directory + top);

    directory_ = directory;

    front_ = front;
    back_ = back;
    left_ = left;
    right_ = right;
    top_ = top;

    for (int i = 0; i < 5; i++) {
        textures_[i]->setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
        textures_[i]->setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        textures_[i]->setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    vbo_.create();
    vbo_.bind();

    glm::vec3 vertices[20] = {
        glm::vec3(size, size, size), glm::vec3(size, -size, size), glm::vec3(-size, size, size), glm::vec3(-size, -size, size), // Front face
        glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size), glm::vec3(size, size, -size), glm::vec3(size, -size, -size), // Back face
        glm::vec3(-size, size, size), glm::vec3(-size, -size, size), glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size), // Left face
        glm::vec3(size, size, -size), glm::vec3(size, -size, -size), glm::vec3(size, size, size), glm::vec3(size, -size, size), // Right face
        glm::vec3(-size, size, -size), glm::vec3(size, size, -size), glm::vec3(-size, size, size), glm::vec3(size, size, size), // Top face
    };

    glm::vec2 texture_coords[4] = {
        glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
    };

    glm::vec3 normals[5] = {
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
    };

    glm::vec4 colour = glm::vec4(1, 1, 1, 1);

    for (int i = 0; i < 20; ++i) {
        vbo_.addData(&vertices[i], sizeof(glm::vec3));
        vbo_.addData(&texture_coords[i%4], sizeof(glm::vec2));
        vbo_.addData(&normals[i/4], sizeof(glm::vec3));
    }

    vbo_.uploadDataToGPU(GL_STATIC_DRAW);

    // Set the vertex attribute locations
    GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *) sizeof(glm::vec3));

    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *) (sizeof(glm::vec3) + sizeof(glm::vec2)));
}

// Render the skybox
void Skybox::render()
{
	Camera *camera = Canvas::instance().camera();
	
	ShaderProgram *main = Canvas::instance().shader_programs(0);
	main->use();
	main->setUniform("toonify", false);
	main->setUniform("texture_fragment", true);

	// Set up a matrix stack
    glutil::MatrixStack modelview = Canvas::instance().modelview();

	Lighting::white();

	modelview.push();
    glm::vec3 eye = camera->position();
    modelview.translate(eye);

    main->setUniform("matrices.modelview", modelview.top());

    glDepthMask(GL_FALSE);
    glBindVertexArray(vao_);

    for (int i = 0; i < 5; ++i) {
        textures_[i]->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }

    glDepthMask(GL_TRUE);
	modelview.pop();
}
