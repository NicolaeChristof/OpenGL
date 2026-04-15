#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace example
{
	class Example
	{
	public:
		Example() {}
		virtual ~Example() {}

		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;
	};

	class ExampleMenu : public Example
	{
	public:
		ExampleMenu(Example*& currentExamplePointer);
		~ExampleMenu();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		template<typename T>
		void RegisterExample(const std::string& name)
		{
			std::cout << "Registering example: " << name << std::endl;
			m_Examples.push_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		Example*& m_CurrentExample;
		std::vector<std::pair<std::string, std::function<Example*()>>> m_Examples;
	};
}