#include "Input.h"

#include "raii/Window.h"

namespace callbacks
{
	static void character(GLFWwindow* window, unsigned int codepoint)
	{
		vg::input::CharEvent event{};
		event.key = (vg::input::Key)codepoint;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.character.on_callback(event);
	}

	static void char_mods(GLFWwindow* window, unsigned int codepoint, int mods)
	{
		vg::input::CharModsEvent event{};
		event.key = (vg::input::Key)codepoint;
		event.mods = mods;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.char_mods.on_callback(event);
	}

	static void cursor_enter(GLFWwindow* window, int entered)
	{
		vg::input::CursorEnterEvent event{};
		event.entered = entered;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.cursor_enter.on_callback(event);
	}

	static void cursor_pos(GLFWwindow* window, double x, double y)
	{
		vg::input::CursorPosEvent event{};
		event.x = x;
		event.y = y;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.cursor_pos.on_callback(event);
	}

	static void drop(GLFWwindow* window, int num_paths, const char** paths)
	{
		vg::input::PathDropEvent event{};
		event.count = num_paths;
		event.paths = paths;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.path_drop.on_callback(event);
	}

	static void framebuffer_size(GLFWwindow* window, int w, int h)
	{
		vg::input::FramebufferResizeEvent event{};
		event.w = w;
		event.h = h;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.framebuffer_resize.on_callback(event);
	}

	static void(*joystick_connection)(vg::input::Controller, bool) = nullptr;

	static void joystick(int jid, int jevent)
	{
		if (joystick_connection)
			joystick_connection(vg::input::Controller(jid), jevent);
	}

	static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		vg::input::KeyEvent event{};
		event.key = (vg::input::Key)key;
		event.scancode = scancode;
		event.action = (vg::input::Action)action;
		event.mods = mods;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.key.on_callback(event);
	}

	static void mouse_button(GLFWwindow* window, int button, int action, int mods)
	{
		vg::input::MouseButtonEvent event{};
		event.button = (vg::input::MouseButton)button;
		event.action = (vg::input::Action)action;
		event.mods = mods;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.mouse_button.on_callback(event);
	}

	static void scroll(GLFWwindow* window, double xoff, double yoff)
	{
		vg::input::ScrollEvent event{};
		event.x = xoff;
		event.y = yoff;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.scroll.on_callback(event);
	}

	static void window_close(GLFWwindow* window)
	{
		vg::input::WindowCloseEvent event{};
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.window_close.on_callback(event);
	}

	static void window_content_scale(GLFWwindow* window, float x_scale, float y_scale)
	{
		vg::input::WindowContentScaleEvent event{};
		event.x = x_scale;
		event.y = y_scale;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.window_content_scale.on_callback(event);
	}

	static void window_focus(GLFWwindow* window, int focused)
	{
		vg::input::WindowFocusEvent event{};
		event.focused = focused;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.window_focus.on_callback(event);
	}

	static void window_iconify(GLFWwindow* window, int iconified)
	{
		vg::input::WindowIconifyEvent event{};
		event.iconified = iconified;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.window_iconify.on_callback(event);
	}

	static void window_maximize(GLFWwindow* window, int maximized)
	{
		vg::input::WindowMaximizeEvent event{};
		event.maximized = maximized;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.window_maximize.on_callback(event);
	}

	static void window_pos(GLFWwindow* window, int x, int y)
	{
		vg::input::WindowPosEvent event{};
		event.x = x;
		event.y = y;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.window_pos.on_callback(event);
	}

	static void window_refresh(GLFWwindow* window)
	{
		vg::input::WindowRefreshEvent event{};
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.window_refresh.on_callback(event);
	}

	static void window_size(GLFWwindow* window, int width, int height)
	{
		vg::input::WindowResizeEvent event{};
		event.w = width;
		event.h = height;
		vg::Window* win = (vg::Window*)glfwGetWindowUserPointer(window);
		win->root_input_handlers.window_resize.on_callback(event);
	}
}

void vg::_::assign_window_callbacks(Window& window)
{
	glfwSetCharCallback(window, callbacks::character);
	glfwSetCharModsCallback(window, callbacks::char_mods);
	glfwSetCursorEnterCallback(window, callbacks::cursor_enter);
	glfwSetCursorPosCallback(window, callbacks::cursor_pos);
	glfwSetDropCallback(window, callbacks::drop);
	glfwSetFramebufferSizeCallback(window, callbacks::framebuffer_size);
	glfwSetKeyCallback(window, callbacks::key);
	glfwSetMouseButtonCallback(window, callbacks::mouse_button);
	glfwSetScrollCallback(window, callbacks::scroll);
	glfwSetWindowCloseCallback(window, callbacks::window_close);
	glfwSetWindowContentScaleCallback(window, callbacks::window_content_scale);
	glfwSetWindowFocusCallback(window, callbacks::window_focus);
	glfwSetWindowIconifyCallback(window, callbacks::window_iconify);
	glfwSetWindowMaximizeCallback(window, callbacks::window_maximize);
	glfwSetWindowPosCallback(window, callbacks::window_pos);
	glfwSetWindowRefreshCallback(window, callbacks::window_refresh);
	glfwSetWindowSizeCallback(window, callbacks::window_size);
}

void vg::_::init_input()
{
	glfwSetJoystickCallback(callbacks::joystick);
}

const char* vg::input::get_key_name(Key key, int scancode)
{
	return glfwGetKeyName((int)key, scancode);
}

int vg::input::get_scancode(Key key)
{
	return glfwGetKeyScancode((int)key);
}

vg::input::Gamepad::Gamepad(Controller c)
	: _c(c)
{
	glfwGetGamepadState((int)_c, &_g);
}

vg::input::Controller vg::input::Gamepad::controller() const
{
	return _c;
}

bool vg::input::Gamepad::is_connected() const
{
	
	return glfwJoystickPresent((int)_c);
}

bool vg::input::Gamepad::has_mapping() const
{
	return glfwJoystickIsGamepad((int)_c);
}

vg::input::Action vg::input::Gamepad::button_state(GamepadButton button) const
{
	return (vg::input::Action)_g.buttons[(int)button];
}

float vg::input::Gamepad::axis_state(GamepadAxis axis) const
{
	return _g.axes[(int)axis];
}

int vg::input::Gamepad::dpad_state(unsigned int dpad) const
{
	int count;
	const unsigned char* hats = glfwGetJoystickHats((int)_c, &count);
	return dpad < (unsigned int)count ? hats[dpad] : 0;
}

const char* vg::input::Gamepad::name() const
{
	return glfwGetJoystickName((int)_c);
}

const char* vg::input::Gamepad::identifier() const
{
	return glfwGetJoystickGUID((int)_c);
}

bool vg::input::update_gamepad_mapping(const char* sdl_game_controller_db)
{
	return glfwUpdateGamepadMappings(sdl_game_controller_db);
}

void vg::input::set_joystick_connection_callback(void(*callback)(Controller, bool))
{
	callbacks::joystick_connection = callback;
}

void(*vg::input::get_joystick_connection_callback())(vg::input::Controller, bool)
{
	return callbacks::joystick_connection;
}
