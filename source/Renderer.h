#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x)   GLClearError();\
                    x;\
                    ASSERT(GLCheckError(__FILE__, #x, __LINE__))

void GLClearError();
bool GLCheckError(const char* file, const char* function, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};