#include "ExampleClearColor.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace example
{
	ExampleClearColor::ExampleClearColor() : m_ClearColor { 0.2f, 0.3f, 0.8f, 1.0f }
	{

	}

	ExampleClearColor::~ExampleClearColor()
	{

	}

	void ExampleClearColor::OnUpdate(float deltaTime)
	{

	}

	void ExampleClearColor::OnRender()
	{
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void ExampleClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}
}