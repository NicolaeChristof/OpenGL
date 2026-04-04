#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x)   GLClearError();\
                    x;\
                    ASSERT(GLCheckError(__FILE__, #x, __LINE__))

void GLClearError();
bool GLCheckError(const char* file, const char* function, int line);