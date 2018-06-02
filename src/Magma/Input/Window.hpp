#pragma once

#include <string>

#include "Event.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace Magma
{
	namespace Input
	{
		enum class WindowMode
		{
			Invalid = -1,

			Windowed,
			Fullscreen,

			Count,
		};

		class Window
		{
		public:
			Window(unsigned int width, unsigned int height, const std::string& title, WindowMode mode = WindowMode::Windowed);
			virtual ~Window();

			void PollEvents();
			void SwapBuffers();

			Event<> OnClose;
			Event<> OnMouseEnter;
			Event<> OnMouseLeave;
			Event<float, float> OnMouseMove;
			Event<float> OnMouseScroll; 
			Event<Keyboard, KeyModifiers> OnKeyUp;
			Event<Keyboard, KeyModifiers> OnKeyDown;
			Event<Mouse> OnMouseUp;
			Event<Mouse> OnMouseDown;

		private:
			void* m_glfwWindow;
		};
	}
}