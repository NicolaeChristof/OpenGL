#pragma once

namespace example
{
	class Example
	{
	public:
		Example() {}
		virtual ~Example() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}