#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    // Generate buffer object names (int size, unsigned int* bufferID)
    GLCall(glGenBuffers(1, &m_RendererID));
    // Bind means to select this buffer as the active vertex buffer
    // Bind a named buffer object (enum target, unsigned int bufferID)
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // Creates and initializes a buffer objects data store (enum target, size (bytes), optional pointer to data, enum usage)
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}