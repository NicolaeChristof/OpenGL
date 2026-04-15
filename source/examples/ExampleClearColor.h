#pragma once

#include "Example.h"

namespace example
{
	class ExampleClearColor : public Example
	{
	public:
		ExampleClearColor();
		~ExampleClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];
	};
}