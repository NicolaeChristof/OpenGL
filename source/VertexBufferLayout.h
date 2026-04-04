#pragma once

#include <vector>
#include "GL/glew.h"
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT			: return sizeof(GLfloat);
			case GL_UNSIGNED_INT	: return sizeof(GLuint);
			case GL_UNSIGNED_BYTE	: return sizeof(GLubyte);
			default					: return 0;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {};
	~VertexBufferLayout() {};

	template<typename T>
	void Push(const unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(const unsigned int count)
	{
		m_Elements.emplace_back(VertexBufferElement(count, GL_FLOAT, GL_FALSE));
		m_Stride += sizeof(GLfloat) * count;
	}

	template<>
	void Push<unsigned int>(const unsigned int count)
	{
		m_Elements.emplace_back(VertexBufferElement(count, GL_UNSIGNED_INT, GL_FALSE));
		m_Stride += sizeof(GLuint) * count;
	}

	template<>
	void Push<unsigned char>(const unsigned int count)
	{
		m_Elements.emplace_back(VertexBufferElement(count, GL_UNSIGNED_BYTE, GL_TRUE));
		m_Stride += sizeof(GLubyte) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline const unsigned int GetStride() const { return m_Stride; }
};