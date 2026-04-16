#pragma once

#include "Example.h"

#include <memory>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace example
{
	class ExampleTexture2D : public Example
	{
	public:
		ExampleTexture2D();
		~ExampleTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Renderer m_Renderer;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Projection;
		glm::vec3 m_ModeltranslationA;
		glm::vec3 m_ModeltranslationB;
		glm::vec3 m_ViewTranslation;
	};
}