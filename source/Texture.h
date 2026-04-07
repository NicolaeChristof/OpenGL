#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width;
	int m_Height;
	int m_BPP; // Bits per pixel
public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0) const; // choose slot to bind, can bind multiple textures
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};