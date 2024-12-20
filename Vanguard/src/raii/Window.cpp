#include "Window.h"

#include "Vanguard.h"
#include "Errors.h"
#include "Input.h"

// LATER use Logger instead
#include <iostream>

vg::Cursor::Cursor(StandardCursor standard_cursor)
{
	_c = glfwCreateStandardCursor((int)standard_cursor);
}

vg::Cursor::Cursor(unsigned char* rgba_pixels, int width, int height, int xhot, int yhot)
{
	GLFWimage image{};
	image.pixels = rgba_pixels;
	image.width = width;
	image.height = height;
	_c = glfwCreateCursor(&image, xhot, yhot);
}

vg::Cursor::Cursor(Cursor&& other) noexcept
	: _c(other._c)
{
	other._c = nullptr;
}

vg::Cursor& vg::Cursor::operator=(Cursor&& other) noexcept
{
	if (this != &other)
	{
		glfwDestroyCursor(_c);
		_c = other._c;
		other._c = nullptr;
	}
	return *this;
}

vg::Cursor::~Cursor()
{
	glfwDestroyCursor(_c);
}

void vg::WindowHint::hint() const
{
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_VISIBLE, visible);
	glfwWindowHint(GLFW_DECORATED, decorated);
	glfwWindowHint(GLFW_FOCUSED, focused);
	glfwWindowHint(GLFW_AUTO_ICONIFY, auto_iconify);
	glfwWindowHint(GLFW_FLOATING, floating);
	glfwWindowHint(GLFW_MAXIMIZED, maximized);
	glfwWindowHint(GLFW_CENTER_CURSOR, center_cursor);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, transparent_framebuffer);
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, focus_on_show);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, scale_to_monitor);
	glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, scale_framebuffer);
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, mouse_passthrough);
	glfwWindowHint(GLFW_POSITION_X, position_x);
	glfwWindowHint(GLFW_POSITION_Y, position_y);
	glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
	glfwWindowHint(GLFW_STEREO, stereo);
	glfwWindowHint(GLFW_SRGB_CAPABLE, srgb_capable);
	glfwWindowHint(GLFW_DOUBLEBUFFER, double_buffer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_version_minor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, opengl_forward_compat);
	glfwWindowHint(GLFW_CONTEXT_DEBUG, context_debug);
	glfwWindowHint(GLFW_OPENGL_PROFILE, opengl_profile);
}

vg::Window::Window(int width, int height, const char* title, const WindowHint& hint)
{
	hint.hint();

	_w = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!_w)
		throw Error(ErrorCode::WINDOW_CREATION);
	focus_context();
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << glewGetErrorString(err) << std::endl;
		glfwDestroyWindow(_w);
		throw Error(ErrorCode::GLEW_INIT);
	}

	glfwSetWindowUserPointer(_w, this);
	_::assign_window_callbacks(*this);

	root_input_handlers.framebuffer_resize.callback = [this](const input::FramebufferResizeEvent& e) {
		glViewport(0, 0, e.w, e.h);
		clear_buffer();
		if (render_during_resize)
			render_during_resize(e.w, e.h);
		swap_buffers();
		};
}

vg::Window::Window(Window&& other) noexcept
	: _w(other._w)
{
	other._w = nullptr;
}

vg::Window& vg::Window::operator=(Window&& other) noexcept
{
	if (this != &other)
	{
		glfwDestroyWindow(_w);
		_w = other._w;
		other._w = nullptr;
	}
	return *this;
}

vg::Window::~Window()
{
	glfwDestroyWindow(_w);
}

void vg::Window::focus() const
{
	glfwFocusWindow(_w);
}

void vg::Window::focus_context() const
{
	glfwMakeContextCurrent(_w);
}

bool vg::Window::should_close() const
{
	return glfwWindowShouldClose(_w);
}

void vg::Window::request_close(bool close) const
{
	glfwSetWindowShouldClose(_w, close);
}

void vg::Window::swap_buffers() const
{
	glfwSwapBuffers(_w);
}

void vg::Window::new_frame() const
{
	VANGUARD_ASSERT_GL_OKAY;
	clear_buffer();
}

void vg::Window::end_frame() const
{
	swap_buffers();
	VANGUARD_ASSERT_GL_OKAY;
}

int vg::Window::width() const
{
	int w;
	glfwGetWindowSize(_w, &w, nullptr);
	return w;
}

void vg::Window::set_width(int width) const
{
	glfwSetWindowSize(_w, width, height());
}

int vg::Window::height() const
{
	int h;
	glfwGetWindowSize(_w, nullptr, &h);
	return h;
}

void vg::Window::set_height(int height) const
{
	glfwSetWindowSize(_w, width(), height);
}

glm::ivec2 vg::Window::size() const
{
	glm::ivec2 size;
	glfwGetWindowSize(_w, &size.x, &size.y);
	return size;
}

void vg::Window::set_size(glm::ivec2 size) const
{
	glfwSetWindowSize(_w, size.x, size.y);
}

double vg::Window::cursor_x() const
{
	double x;
	glfwGetCursorPos(_w, &x, nullptr);
	return x;
}

void vg::Window::cursor_x(double cursor_x) const
{
	glfwSetCursorPos(_w, cursor_x, cursor_y());
}

double vg::Window::cursor_y() const
{
	double y;
	glfwGetCursorPos(_w, nullptr, &y);
	return y;
}

void vg::Window::cursor_y(double cursor_y) const
{
	glfwSetCursorPos(_w, cursor_x(), cursor_y);
}

glm::dvec2 vg::Window::cursor_pos() const
{
	glm::dvec2 pos;
	glfwGetCursorPos(_w, &pos.x, &pos.y);
	return pos;
}

void vg::Window::set_cursor_pos(glm::dvec2 pos) const
{
	glfwSetCursorPos(_w, pos.x, pos.y);
}

bool vg::Window::is_key_pressed(input::Key key) const
{
	return glfwGetKey(_w, int(key)) == int(input::Action::PRESS);
}

bool vg::Window::is_shift_pressed() const
{
	return is_key_pressed(input::Key::SHIFT_LEFT) || is_key_pressed(input::Key::SHIFT_RIGHT);
}

bool vg::Window::is_ctrl_pressed() const
{
	return is_key_pressed(input::Key::CONTROL_LEFT) || is_key_pressed(input::Key::CONTROL_RIGHT);
}

bool vg::Window::is_alt_pressed() const
{
	return is_key_pressed(input::Key::ALT_LEFT) || is_key_pressed(input::Key::ALT_RIGHT);
}

bool vg::Window::is_super_pressed() const
{
	return is_key_pressed(input::Key::SUPER_LEFT) || is_key_pressed(input::Key::SUPER_RIGHT);
}

bool vg::Window::is_mouse_button_pressed(input::MouseButton mb) const
{
	return glfwGetMouseButton(_w, int(mb)) == int(input::Action::PRESS);
}

vg::MouseMode vg::Window::mouse_mode() const
{
	return (vg::MouseMode)glfwGetInputMode(_w, GLFW_CURSOR);
}

void vg::Window::set_mouse_mode(MouseMode mode) const
{
	glfwSetInputMode(_w, GLFW_CURSOR, (int)mode);
}

void vg::Window::set_cursor(Cursor& cursor) const
{
	glfwSetCursor(_w, cursor);
}
