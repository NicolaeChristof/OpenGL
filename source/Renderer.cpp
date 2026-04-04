#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* file, const char* function, int line)
{
    static bool success = true;
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << file << " " << function << " : " << line << std::endl;
        success = false;
    }
    return success;
}