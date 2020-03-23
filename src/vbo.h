#ifndef VBO_H
#define VBO_H

#include "common.h"

// This class provides a wrapper around an OpenGL Vertex Buffer Object
class VBO
{
public:
    VBO();

    // Creates a VBO
    void create(int size = 0);

    // Binds the VBO
    void bind(int type = GL_ARRAY_BUFFER);

    // Releases the VBO
    void release();

    // Add data to the VBO
    void addData(void* data, GLuint size);

    // Upload the VBO to the GPU
    void uploadDataToGPU(int usage);

    void *mapBufferToMemory(int usage);
    void *mapSubBufferToMemory(int usage, GLuint offset, GLuint length);
    void unmapBuffer();

    void* data();
    GLuint buffer();
  
private:
    GLuint buffer_;
    int size_;
    int type_;
    std::vector<BYTE> data_;

    bool uploaded_;
};

#endif
