#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* file, const char* function, int line)
{
    bool success = true;
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << file << " " << function << " : " << line << std::endl;
        success = false;
    }
    return success;
}

// Renderer class functions

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    // Draw the currently bound buffer
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0));
}