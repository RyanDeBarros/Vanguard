#pragma once

#include <unordered_map>

#include "Vendor.h"
#include "Input.h"

namespace vg
{
	struct WindowHint
	{
		bool resizable = true;
		bool visible = true;
		bool decorated = true;
		bool focused = true;
		bool auto_iconify = true;
		bool floating = false;
		bool maximized = false;
		bool center_cursor = true;
		bool transparent_framebuffer = false;
		bool focus_on_show = true;
		bool scale_to_monitor = false;
		bool scale_framebuffer = true;
		bool mouse_passthrough = false;
		unsigned int position_x = GLFW_ANY_POSITION;
		unsigned int position_y = GLFW_ANY_POSITION;
		
		int refresh_rate = GLFW_DONT_CARE;
		bool stereo = false;
		bool srgb_capable = false;
		bool double_buffer = true;

		int context_version_major = 4; // 1
		int context_version_minor = 4; // 0
		bool opengl_forward_compat = false;
		bool context_debug = false;
		int opengl_profile = GLFW_OPENGL_CORE_PROFILE; // GLFW_OPENGL_ANY_PROFILE

		void hint() const;
	};

	class Window
	{
		GLFWwindow* _w = nullptr;

	public:
		Window(int width, int height, const char* title, const WindowHint& hint = {});
		Window(const Window&) = delete;
		Window(Window&&) noexcept;
		Window& operator=(Window&&) noexcept;
		~Window();
		operator const GLFWwindow* () const;
		operator GLFWwindow* ();

		void focus() const;
		void focus_context() const;
		bool should_close() const;
		void request_close(bool close = true) const;
		void swap_buffers() const;

		void new_frame() const;
		void end_frame() const;

		int width() const;
		void set_width(int width) const;
		int height() const;
		void set_height(int height) const;
		glm::ivec2 size() const;
		void set_size(glm::ivec2 size) const;

		double cursor_x() const;
		void cursor_x(double cursor_x) const;
		double cursor_y() const;
		void cursor_y(double cursor_y) const;
		glm::dvec2 cursor_pos() const;
		void set_cursor_pos(glm::dvec2 pos) const;

		struct
		{
			input::CharEventHandler character;
			input::CharModsEventHandler char_mods;
			input::CursorEnterEventHandler cursor_enter;
			input::CursorPosEventHandler cursor_pos;
			input::PathDropEventHandler path_drop;
			input::FramebufferResizeEventHandler framebuffer_resize;
			input::KeyEventHandler key;
			input::MouseButtonEventHandler mouse_button;
			input::ScrollEventHandler scroll;
			input::WindowCloseEventHandler window_close;
			input::WindowContentScaleEventHandler window_content_scale;
			input::WindowFocusEventHandler window_focus;
			input::WindowIconifyEventHandler window_iconify;
			input::WindowMaximizeEventHandler window_maximize;
			input::WindowPosEventHandler window_pos;
			input::WindowRefreshEventHandler window_refresh;
			input::WindowResizeEventHandler window_resize;
		} root_input_handlers = {};
	};
}
