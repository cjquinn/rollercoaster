#include "terrain.h"

#include "include/gl/glew.h"
#include "include/gl/glaux.h"

#include "vertex.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
	delete[] heightmap_;
}

glm::vec3 Terrain::imageToWorld(glm::vec3 p)
{
  p.x = 2.0f * (p.x / width_) - 1.0f;
  p.z = 2.0f * (p.z / height_) - 1.0f;

  p.x *= size_x_ / 2.0f;
  p.z *= size_z_ / 2.0f;

  return p;
}

glm::vec3 Terrain::worldToImage(glm::vec3 p)
{
  p.x *= 2.0f / size_x_;
  p.z *= 2.0f / size_z_;

  p.x = p.x + 1.0f * (width_ / 2.0f);
  p.z = p.z + 1.0f * (height_ / 2.0f);

  return p;
}

bool Terrain::create(char *filename, float size_x, float size_z, float scale)
{
	FILE *file = fopen(filename, "r");
	if(!file) {
		return false;
	}
	fclose(file);
	
	AUX_RGBImageRec* image;
	image = auxDIBImageLoad(filename);

	width_ = image->sizeX;
	height_ = image->sizeY;
	size_x_ = size_x;
	size_z_ = size_z;

	heightmap_ = new float[width_ * height_];
	if(heightmap_ == NULL) {
		return false;
	}

	memset(heightmap_, 0, width_ * height_ * sizeof(float));

	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	for (int z = 0; z < height_; ++z) {
    for (int x = 0; x < width_; ++x) {
      int index = x + z * width_;

      float grayscale = (image->data[index * 3] + image->data[index * 3 + 1] + image->data[index * 3 + 2]) / 3.0f;
      float height = (grayscale - 128.0f) / 128.0f;

      glm::vec3 pixel = glm::vec3((float) x, height, (float) z);
      glm::vec3 world = imageToWorld(pixel);
   
			world.y *= scale;
      heightmap_[index] = world.y;

      Vertex v(world, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
      vertices.push_back(v);
    }
  }

  for (int z = 0; z < height_ - 1; ++z) {
    for (int x = 0; x < width_ - 1; ++x) {
      int index = x + z * width_;

      triangles.push_back(index);
      triangles.push_back(index + 1 + width_);
      triangles.push_back(index + 1);

      triangles.push_back(index);
      triangles.push_back(index + width_);
      triangles.push_back(index + 1 + width_);
    }
  }

	// Load the texture 14 Feb 2013
  texture_.load("resources\\textures\\snow.jpg", true);

  // Set parameters for texturing using sam pler object
  texture_.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);

  mesh_.create(texture_, vertices, triangles);

	if (image) {
    if (image->data) {
      free(image->data);
    }
    free(image);
	}

  return true;
}

float Terrain::getTerrainHeight(glm::vec3 p)
{
  glm::vec3 image = worldToImage(p);

  int index = (int) (image.x + 0.5) + (int) (image.z + 0.5) * width_;
  if (index < 0 || index > width_ * height_) {
    return 0.0f;
  } else {
    return heightmap_[index];
  }
}

void Terrain::render()
{
	mesh_.render();
}
