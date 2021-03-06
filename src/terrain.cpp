#include "terrain.h"

#include <iostream>

#include "canvas.h"
#include "lighting.h"
#include "matrixstack.h"
#include "mesh.h"
#include "shaderprogram.h"
#include "texture.h"
#include "vertex.h"

Terrain::Terrain() : dib_(NULL), mesh_(NULL), texture_(NULL)
{}

Terrain::~Terrain()
{
	delete [] heightmap_;
	delete mesh_;
	delete texture_;
}

glm::vec3 Terrain::imageToWorld(glm::vec3 p)
{
	p.x = 2.0f * (p.x / width_) - 1.0f;
	p.z = 2.0f * (p.z / height_) - 1.0f;

	p.x *= size_x_ / 2.0f;
	p.z *= size_z_ / 2.0f;

	p += origin_;

	return p;

}

glm::vec3 Terrain::worldToImage(glm::vec3 p)
{
	p -= origin_;

	p.x *= 2.0f / size_x_;
	p.z *= 2.0f / size_z_;

	p.x = (p.x + 1.0f) * (width_ / 2.0f);
	p.z = (p.z + 1.0f) * (height_ / 2.0f);

	return p;
}

void Terrain::create(std::string heightmap, glm::vec3 origin, float size_x, float size_z, float scale)
{
	mesh_ = new Mesh;
	texture_ = new Texture;
	texture_->load(ROOT_DIR + std::string("/resources/textures/terrain.jpg"));
	texture_->setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
  	texture_->setSamplerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
  	texture_->setSamplerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glm::vec2 texture_coords[4] = {
    	glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
  	};

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(heightmap.c_str(), 0);

	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(heightmap.c_str());
	}

	dib_ = FreeImage_Load(fif, heightmap.c_str());
	BYTE *data_pointer = FreeImage_GetBits(dib_);

	width_ = FreeImage_GetWidth(dib_);
	height_ = FreeImage_GetHeight(dib_);

	origin_ = origin;
	size_x_ = size_x;
	size_z_ = size_z;
	
	heightmap_ = new float[width_ * height_];

	memset(heightmap_, 0, width_ * height_ * sizeof(float));

	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	for (int z = 0; z < height_; z++) {
		for (int x = 0; x < width_; x++) {
			int index = x + z * width_;

			float grayscale = (data_pointer[index * 3] + data_pointer[index * 3 + 1] + data_pointer[index * 3 + 2]) / 3.0f;
			float height = (grayscale - 128.0f) / 128.0f;

			glm::vec3 image = glm::vec3(x, height, z);
			glm::vec3 world = imageToWorld(image);

			world.y *= scale;	 
			heightmap_[index] = world.y;

			Vertex v = Vertex(world, glm::vec3(0.0f, 0.0f, 0.0f), texture_coords[index % 4]);
			vertices.push_back(v);
		}
	}

	FreeImage_Unload(dib_);

	for (int z = 0; z < height_ - 1; z++) {
		for (int x = 0; x < width_ - 1; x++) {
			int index = x + z * width_;
			
			triangles.push_back(index);
			triangles.push_back(index + 1 + width_);
			triangles.push_back(index + 1);

			triangles.push_back(index);
			triangles.push_back(index + width_);
			triangles.push_back(index + 1 + width_);
		}
	}

	mesh_->create(vertices, triangles, false);
}

float Terrain::groundHeight(glm::vec3 p)
{
	glm::vec3 image = worldToImage(p);

	int x = (int) floor(image.x);
	int z = (int) floor (image.z);
	
	if (x < 0 || x >= width_ - 1 || z < 0 || z >= height_ - 1) {
		return 0.0f;
	}

	int indexl = x + z * width_;
	int indexr = (x+1) + z * width_;
	int indexul = x + (z+1) * width_;
	int indexur = (x+1) + (z+1) * width_;

	float dx = image.x - x;
	float dz = image.z - z;

	float a = (1-dx) * heightmap_[indexl] + dx * heightmap_[indexr];
	float b = (1-dx) * heightmap_[indexul] + dx * heightmap_[indexur];
	float c = (1-dz) * a + dz * b;
	return c;
}

void Terrain::render()
{
	ShaderProgram *main = Canvas::instance().shader_programs(0);
	main->use();
	main->setUniform("toonify", true);
	main->setUniform("texture_fragment", false);

	texture_->bind();

	// Set up a matrix stack
  	glutil::MatrixStack modelview = Canvas::instance().modelview();

	// Set light and materials in main shader program
  	glm::vec4 light_position(0, 100, 0, 1);
  	glm::vec4 light_eye = modelview.top() * light_position;

	Lighting::setPosition(light_eye);
	Lighting::setReflectance(glm::vec3(0.352f, 0.443f, 0.654f), glm::vec3(0.247f, 0.356f, 0.603f), glm::vec3(0.1f), 15.0f);

	modelview.push();
	main->setUniform("matrices.modelview", modelview.top());
	mesh_->render();
	modelview.pop();
}
