#include "Example.h"

#include "imgui/imgui.h"
#include <iostream>

namespace example
{
	ExampleMenu::ExampleMenu(Example*& currentExamplePointer) : m_CurrentExample(currentExamplePointer)
	{

	}

	ExampleMenu::~ExampleMenu()
	{

	}

	void ExampleMenu::OnUpdate(float deltaTime)
	{

	}

	void ExampleMenu::OnRender()
	{

	}

	void ExampleMenu::OnImGuiRender()
	{
		for (std::pair<std::string, std::function<Example* ()>> example : m_Examples)
		{
			if (ImGui::Button(example.first.c_str()))
			{
				m_CurrentExample = example.second();
			}
		}
	}
}