#include "fbo.h"

#include "texture.h"

FBO::FBO() : framebuffer_(0), depth_renderbuffer_(0)
{}

FBO::~FBO()
{
	if(framebuffer_) {
		glDeleteFramebuffers(1, &framebuffer_);
		framebuffer_ = 0;
	}

	if (depth_renderbuffer_) {
		glDeleteRenderbuffers(1, &depth_renderbuffer_);
		depth_renderbuffer_ = 0;
	}

	glDeleteSamplers(1, &sampler_);
	glDeleteTextures(1, &texture_);
}

void FBO::createEmptyTexture(int width, int height, int ibbp, GLenum format)
{
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	
	if (format == GL_RGBA || format == GL_BGRA) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	} else if (format == GL_RGB || format == GL_BGR) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, width, 0, format, GL_UNSIGNED_BYTE, NULL);
	}

	glGenSamplers(1, &sampler_);
}

bool FBO::create(int width, int height)
{
	if (framebuffer_ != 0) {
		return false;
	}

	glGenFramebuffers(1, &framebuffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

	createEmptyTexture(width, height, 24, GL_RGB);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);

	width_ = width;
	height_ = height;

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

// Add a depth buffer for hidden surface removal
bool FBO::addDepthBuffer()
{
	if (framebuffer_ == 0) {
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

	glGenRenderbuffers(1, &depth_renderbuffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_renderbuffer_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width_, height_);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer_);

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

// Bind the FBO so we can render to it
void FBO::bind(bool full_viewport)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
	if (full_viewport) {
		glViewport(0, 0, width_, height_);
	}
}

// Beind the frambuffer texture so it is active
void FBO::bindTexture(int texture_id, bool regen_mipmaps)
{
	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glBindSampler(texture_id, sampler_);

	if (regen_mipmaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void FBO::setTextureFiltering(int magnification, int minification)
{
	if (magnification == TEXTURE_FILTER_MAG_NEAREST) {
		glSamplerParameteri(sampler_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}	else if (magnification == TEXTURE_FILTER_MAG_BILINEAR) {
		glSamplerParameteri(sampler_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	if (minification == TEXTURE_FILTER_MIN_NEAREST) {
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	} else if (minification == TEXTURE_FILTER_MIN_BILINEAR) {
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	} else if (minification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP) {
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	} else if (minification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP) {
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	} else if(minification == TEXTURE_FILTER_MIN_TRILINEAR) {
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
}

glm::mat4 FBO::calculatePerspectiveProjection(float fFOV, float fNear, float fFar)
{
	return glm::perspective(fFOV, float(width_)/float(height_), fNear, fFar);
}

glm::mat4 FBO::calculateOrthographicProjection()
{
	return glm::ortho(0.0f, float(width_), 0.0f, float(height_));
}


int FBO::width()
{
	return width_;
}

int FBO::height()
{
	return height_;
}