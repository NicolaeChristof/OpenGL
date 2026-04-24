#include "ExampleTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace example
{
	ExampleTexture2D::ExampleTexture2D() : m_Projection(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)), 
        m_ModelTranslationA(-0.5f, 0.0f, 0.0f), m_ModelTranslationB(0.5f, 0.0f, 0.0f), m_ViewTranslation(0.0f, 0.0f, 0.0f)
	{
        // vertexPos.x, vertexPos.y, textureCoord.x, textureCoord.y
        float positions[] =
        {
            -0.25f, -0.25f, 0.0f, 0.0f, // 0
             0.25f, -0.25f, 1.0f, 0.0f, // 1
             0.25f,  0.25f, 1.0f, 1.0f, // 2
            -0.25f,  0.25f, 0.0f, 1.0f  // 3
        };
#if _DEBUG
        std::cout << "sizeof positions: " << sizeof(positions) << std::endl;
#endif
        // vertex indicies
        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };
#if _DEBUG
        std::cout << "sizeof indicies: " << sizeof(indices) << std::endl;
        std::cout << "count of indicies: " << std::size(indices) << std::endl;
#endif

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push<float>(2); // 2 floats for vertex position
        layout.Push<float>(2); // 2 floats for texture coordinates
        m_VAO->AddBuffer(*m_VBO, layout);
        
        m_IBO = std::make_unique<IndexBuffer>(indices, std::size(indices));

        m_Texture = std::make_unique<Texture>("resources/textures/MadeInAbyss.png");
        m_Texture->Bind();

        m_Shader = std::make_unique<Shader>("resources/shaders/Basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	ExampleTexture2D::~ExampleTexture2D()
	{
		
	}

	void ExampleTexture2D::OnUpdate(float deltaTime)
	{

	}

	void ExampleTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_Shader->Bind();

        // render texture A
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelTranslationA);
            glm::mat4 view = glm::translate(glm::mat4(1.0f), -m_ViewTranslation);
            glm::mat4 mvp = m_Projection * view * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            m_Renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
        // render texture B
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelTranslationB);
            glm::mat4 view = glm::translate(glm::mat4(1.0f), -m_ViewTranslation);
            glm::mat4 mvp = m_Projection * view * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            m_Renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
	}

	void ExampleTexture2D::OnImGuiRender()
	{
        ImGui::Text("Move the sliders to modify the texture positions!");

        ImGui::SliderFloat2("Model A", &m_ModelTranslationA.x, -1.0f, 1.0f); // Edit 1 float using a slider from lower bounds to upper bounds
        ImGui::SliderFloat2("Model B", &m_ModelTranslationB.x, -1.0f, 1.0f); // Edit 1 float using a slider from lower bounds to upper bounds

        ImGui::SliderFloat2("'Camera'", &m_ViewTranslation.x, -1.0f, 1.0f); // Edit 1 float using a slider from lower bounds to upper bounds
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}