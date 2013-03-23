#ifndef FBO_H
#define FBO_H

#include "shader.h"
#include "texture.h"

class FBO
{
public:
	FBO();
	~FBO();

	// Create a framebuffer object with a texture of a given size
	bool create(int width, int height);

	bool addDepthBuffer();

	void bind(bool full_viewport = true);
	
	void setTextureFiltering(int magnification, int minification);
	void bindTexture(int texture_id = 0, bool regen_mipmaps = false);

	glm::mat4 calculateOrthographicProjection();
	glm::mat4 calculatePerspectiveProjection(float fov, float near, float far);

	int width();
	int height();

private:

	void createEmptyTexture(int width, int height, int ibbp, GLenum format);

	int width_, height_;
	UINT framebuffer_;
	UINT depth_renderbuffer_;
	UINT texture_;
	UINT sampler_;

};

#endif