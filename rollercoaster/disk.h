#ifndef DISK_H
#define DISK_H

#include "frame.h"
#include "texture.h"
#include "vbo.h"

// Class for generating a unit disk
class Disk
{
public:
	Disk();
	~Disk();
	void create(std::string directory, std::string file, unsigned int samples, Frame frame);
	void render();
	void release();
	private:
	GLuint vao_;
	VBO vbo_;
	Texture texture_;
	unsigned int vertices_;
};

#endif