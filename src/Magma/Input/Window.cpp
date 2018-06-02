#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <sstream>
#include <map>

static std::map<GLFWwindow*, Magma::Input::Window*> glfwWindows;

Magma::Input::Keyboard GLFWToMagmaKey(int key)
{
	switch (key)
	{
		case GLFW_KEY_Q: return Magma::Input::Keyboard::Q;
		case GLFW_KEY_W: return Magma::Input::Keyboard::W;
		case GLFW_KEY_E: return Magma::Input::Keyboard::E;
		case GLFW_KEY_R: return Magma::Input::Keyboard::R;
		case GLFW_KEY_T: return Magma::Input::Keyboard::T;
		case GLFW_KEY_Y: return Magma::Input::Keyboard::Y;
		case GLFW_KEY_U: return Magma::Input::Keyboard::U;
		case GLFW_KEY_I: return Magma::Input::Keyboard::I;
		case GLFW_KEY_O: return Magma::Input::Keyboard::O;
		case GLFW_KEY_P: return Magma::Input::Keyboard::P;
		case GLFW_KEY_A: return Magma::Input::Keyboard::A;
		case GLFW_KEY_S: return Magma::Input::Keyboard::S;
		case GLFW_KEY_D: return Magma::Input::Keyboard::D;
		case GLFW_KEY_F: return Magma::Input::Keyboard::F;
		case GLFW_KEY_G: return Magma::Input::Keyboard::G;
		case GLFW_KEY_H: return Magma::Input::Keyboard::H;
		case GLFW_KEY_J: return Magma::Input::Keyboard::J;
		case GLFW_KEY_K: return Magma::Input::Keyboard::K;
		case GLFW_KEY_L: return Magma::Input::Keyboard::L;
		case GLFW_KEY_Z: return Magma::Input::Keyboard::Z;
		case GLFW_KEY_X: return Magma::Input::Keyboard::X;
		case GLFW_KEY_C: return Magma::Input::Keyboard::C;
		case GLFW_KEY_V: return Magma::Input::Keyboard::V;
		case GLFW_KEY_B: return Magma::Input::Keyboard::B;
		case GLFW_KEY_N: return Magma::Input::Keyboard::N;
		case GLFW_KEY_M: return Magma::Input::Keyboard::M;

		case GLFW_KEY_1: return Magma::Input::Keyboard::Num1;
		case GLFW_KEY_2: return Magma::Input::Keyboard::Num2;
		case GLFW_KEY_3: return Magma::Input::Keyboard::Num3;
		case GLFW_KEY_4: return Magma::Input::Keyboard::Num4;
		case GLFW_KEY_5: return Magma::Input::Keyboard::Num5;
		case GLFW_KEY_6: return Magma::Input::Keyboard::Num6;
		case GLFW_KEY_7: return Magma::Input::Keyboard::Num7;
		case GLFW_KEY_8: return Magma::Input::Keyboard::Num8;
		case GLFW_KEY_9: return Magma::Input::Keyboard::Num9;
		case GLFW_KEY_0: return Magma::Input::Keyboard::Num0;

		case GLFW_KEY_F1: return Magma::Input::Keyboard::F1;
		case GLFW_KEY_F2: return Magma::Input::Keyboard::F2;
		case GLFW_KEY_F3: return Magma::Input::Keyboard::F3;
		case GLFW_KEY_F4: return Magma::Input::Keyboard::F4;
		case GLFW_KEY_F5: return Magma::Input::Keyboard::F5;
		case GLFW_KEY_F6: return Magma::Input::Keyboard::F6;
		case GLFW_KEY_F7: return Magma::Input::Keyboard::F7;
		case GLFW_KEY_F8: return Magma::Input::Keyboard::F8;
		case GLFW_KEY_F9: return Magma::Input::Keyboard::F9;
		case GLFW_KEY_F10: return Magma::Input::Keyboard::F10;
		case GLFW_KEY_F11: return Magma::Input::Keyboard::F11;
		case GLFW_KEY_F12: return Magma::Input::Keyboard::F12;

		case GLFW_KEY_ESCAPE: return Magma::Input::Keyboard::Escape;
		case GLFW_KEY_TAB: return Magma::Input::Keyboard::Tab;
		case GLFW_KEY_CAPS_LOCK: return Magma::Input::Keyboard::Caps;
		case GLFW_KEY_LEFT_SHIFT: return Magma::Input::Keyboard::LShift;
		case GLFW_KEY_RIGHT_SHIFT: return Magma::Input::Keyboard::RShift;
		case GLFW_KEY_LEFT_CONTROL: return Magma::Input::Keyboard::LControl;
		case GLFW_KEY_RIGHT_CONTROL: return Magma::Input::Keyboard::RControl;
		case GLFW_KEY_LEFT_ALT: return Magma::Input::Keyboard::Alt;
		case GLFW_KEY_RIGHT_ALT: return Magma::Input::Keyboard::AltGr;
		case GLFW_KEY_SPACE: return Magma::Input::Keyboard::Space;
		case GLFW_KEY_ENTER: return Magma::Input::Keyboard::Enter;
		case GLFW_KEY_BACKSPACE: return Magma::Input::Keyboard::Backspace;
		case GLFW_KEY_INSERT: return Magma::Input::Keyboard::Insert;
		case GLFW_KEY_DELETE: return Magma::Input::Keyboard::Delete;
		case GLFW_KEY_HOME: return Magma::Input::Keyboard::Home;
		case GLFW_KEY_END: return Magma::Input::Keyboard::End;
		case GLFW_KEY_PAGE_UP: return Magma::Input::Keyboard::PageUp;
		case GLFW_KEY_PAGE_DOWN: return Magma::Input::Keyboard::PageDown;

		default: return Magma::Input::Keyboard::Invalid;
	}
}

void GLFWWindowCloseCallback(GLFWwindow* window)
{
	glfwWindows[window]->OnClose.Fire();
}

void GLFWErrorCallback(int err, const char* errMsg)
{
	std::stringstream ss;
	ss << "GLFW error caught (" << err << "): " << errMsg;
	throw std::runtime_error(ss.str());
}

void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto k = GLFWToMagmaKey(key);

	switch (action)
	{
		case GLFW_PRESS:
			glfwWindows[window]->OnKeyDown.Fire(k, (Magma::Input::KeyModifiers)mods);
			break;
		case GLFW_RELEASE:
			glfwWindows[window]->OnKeyUp.Fire(k, (Magma::Input::KeyModifiers)mods);
			break;
	}
}

void GLFWMousePositionCallback(GLFWwindow* window, double x, double y)
{
	glfwWindows[window]->OnMouseMove.Fire((int)x, (int)y);
}

void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto mouseButton = Magma::Input::Mouse::Invalid;

	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
			mouseButton = Magma::Input::Mouse::Left;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouseButton = Magma::Input::Mouse::Right;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			mouseButton = Magma::Input::Mouse::Middle;
			break;
	}

	switch (action)
	{
		case GLFW_PRESS:
			glfwWindows[window]->OnMouseDown.Fire(mouseButton);
			break;
		case GLFW_RELEASE:
			glfwWindows[window]->OnMouseUp.Fire(mouseButton);
			break;
	}
}

void GLFWMouseScroll(GLFWwindow* window, double x, double y)
{
	glfwWindows[window]->OnMouseScroll.Fire((int)y);
}

void GLFWCursorEnterCallback(GLFWwindow* window, int enter)
{
	if (enter == GLFW_TRUE)
		glfwWindows[window]->OnMouseEnter.Fire();
	else
		glfwWindows[window]->OnMouseLeave.Fire();
}

Magma::Input::Window::Window(unsigned int width, unsigned int height, const std::string& title, WindowMode mode)
{
	if (glfwWindows.size() == 0)
	{
		glfwSetErrorCallback(GLFWErrorCallback);

		auto err = glfwInit();
		if (err != GLFW_TRUE)
		{
			std::stringstream ss;
			ss << "Failed to open window, couldn't init GLFW";
			throw std::runtime_error(ss.str());
		}
	}

	GLFWwindow* win;

	glfwWindowHint(GLFW_RESIZABLE, 0);

	switch (mode)
	{
		case WindowMode::Windowed:
			win = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
			break;

		case WindowMode::Fullscreen:
			win = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
			break;
	}

	if (win == nullptr)
		throw std::runtime_error("Failed to open window, window is NULL");

	glfwMakeContextCurrent(win);

	glfwSetKeyCallback(win, GLFWKeyCallback);
	glfwSetCursorPosCallback(win, GLFWMousePositionCallback);
	glfwSetWindowCloseCallback(win, GLFWWindowCloseCallback);
	glfwSetScrollCallback(win, GLFWMouseScroll);
	glfwSetCursorEnterCallback(win, GLFWCursorEnterCallback);
	
	m_glfwWindow = win;
	glfwWindows[win] = this;
}

Magma::Input::Window::~Window()
{
	glfwWindows.erase((GLFWwindow*)m_glfwWindow);
	glfwDestroyWindow((GLFWwindow*)m_glfwWindow);

	if (glfwWindows.size() == 0)
		glfwTerminate();
}

void Magma::Input::Window::PollEvents()
{
	glfwPollEvents();
}

void Magma::Input::Window::SwapBuffers()
{
	glfwSwapBuffers((GLFWwindow*)m_glfwWindow);
}
