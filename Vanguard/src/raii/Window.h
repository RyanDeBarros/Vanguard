#pragma once

#include <unordered_map>
#include <functional>

#include "Vanguard.h"
#include "Vendor.h"
#include "Input.h"
#include "Shader.h"

namespace vg
{
	enum class MouseMode
	{
		VISIBLE = GLFW_CURSOR_NORMAL,
		HIDDEN = GLFW_CURSOR_HIDDEN,
		VIRTUAL = GLFW_CURSOR_DISABLED,
		CAPTURED = GLFW_CURSOR_CAPTURED
	};

	enum class StandardCursor
	{
		ARROW = GLFW_ARROW_CURSOR,
		IBEAM = GLFW_IBEAM_CURSOR,
		CROSSHAIR = GLFW_CROSSHAIR_CURSOR,
		HAND = GLFW_POINTING_HAND_CURSOR,
		RESIZE_EW = GLFW_RESIZE_EW_CURSOR,
		RESIZE_NS = GLFW_RESIZE_NS_CURSOR,
		RESIZE_NW_SE = GLFW_RESIZE_NWSE_CURSOR,
		RESIZE_NE_SW = GLFW_RESIZE_NESW_CURSOR,
		RESIZE_OMNI = GLFW_RESIZE_ALL_CURSOR,
		CANCEL = GLFW_NOT_ALLOWED_CURSOR
	};

	class Cursor
	{
		GLFWcursor* _c = nullptr;

	public:
		Cursor(StandardCursor standard_cursor);
		Cursor(unsigned char* rgba_pixels, int width, int height, int xhot = 0, int yhot = 0);
		Cursor(const Cursor&) = delete;
		Cursor(Cursor&&) noexcept;
		Cursor& operator=(Cursor&&) noexcept;
		~Cursor();

		operator const GLFWcursor* () const { return _c; }
		operator GLFWcursor* () { return _c; }
	};

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

		int context_version_major = VANGUARD_MIN_OPENGL_VERSION_MAJOR; // 1
		int context_version_minor = VANGUARD_MIN_OPENGL_VERSION_MINOR; // 0
		bool opengl_forward_compat = false;
		bool context_debug = false;
		int opengl_profile = GLFW_OPENGL_CORE_PROFILE; // GLFW_OPENGL_ANY_PROFILE

		void hint() const;
	};

	struct ContextConfig
	{
		bool blending = true;
		bool scissor_test = true;
		bool vsync_on = true;

		BlendFactor blend_src_factor = BlendFactor::SRC_ALPHA;
		BlendFactor blend_dst_factor = BlendFactor::ONE_MINUS_SRC_ALPHA;
	};

	enum class DisplayMode
	{
		STRETCH,
		KEEP_SCALE,
		ASPECT
	};

	struct WindowInitialValues
	{
		float desired_aspect_ratio_override = -1;
		DisplayMode display_mode = DisplayMode::ASPECT;
		float scale_width = 1.0f;
		float scale_height = 1.0f;
	};

	class Window
	{
		GLFWwindow* _w = nullptr;

	public:
		Window(int width, int height, const char* title, const WindowInitialValues& initial_values = {}, const WindowHint& hint = {}, const ContextConfig& config = {});
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		~Window();

		operator const GLFWwindow* () const { return _w; }
		operator GLFWwindow* () { return _w; }

	private:
		void init_gl_constants() const;

	public:
		void focus() const;
		void focus_context() const;
		bool should_close() const;
		void request_close(bool close = true) const;
		void swap_buffers() const;
		void unbind_framebuffer() const;

		void new_frame() const;
		void end_frame() const;
		std::function<void()> render_frame = []() {};
		void frame_cycle() const;

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

		enum class CoordinateSystem
		{
			CLIP,
			SCREEN,
			UI
		};
		glm::vec2 convert_coordinates(glm::vec2 coordinates, CoordinateSystem from, CoordinateSystem to) const;

		enum class ProjectionMode
		{
			ORTHOGRAPHIC_2D,
			ORTHOGRAPHIC_3D,
			PERSPECTIVE_2D,
			PERSPECTIVE_3D
		};
		glm::mat3 orthographic_projection() const;
		glm::mat4 orthographic_projection(float z_near, float z_far) const;
		glm::mat4 perspective_projection(float fov, float z_near, float z_far, glm::vec2 aspect_scale = { 1.0f, 1.0f }) const;

		bool is_key_pressed(input::Key key) const;
		bool is_shift_pressed() const;
		bool is_ctrl_pressed() const;
		bool is_alt_pressed() const;
		bool is_super_pressed() const;
		bool is_mouse_button_pressed(input::MouseButton mb) const;

		MouseMode mouse_mode() const;
		void set_mouse_mode(MouseMode mode) const;
		void set_cursor(Cursor& cursor) const;

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

	private:
		struct GLConstants
		{
			GLuint max_texture_image_units = 0;
		} gl_constants = {};

	public:
		const GLConstants& constants() const { return gl_constants; }

		glm::vec4 clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };

		DisplayMode display_mode = DisplayMode::ASPECT;

		int initial_w, initial_h;
		float scale_width = 1.0f;
		float scale_height = 1.0f;
		float desired_aspect_ratio;
		
		struct ShaderVPUpdate
		{
			raii::Shader* shader;
			ProjectionMode proj_mode;
			float z_near, z_far, fov;
			glm::vec2 aspect_scale = { 1.0f, 1.0f };
		};
		std::vector<ShaderVPUpdate> vp_updates;

	private:
		Rect<int> viewport;

	public:

		void sync_resize(int w, int h);
		void sync_resize() { auto sz = size(); sync_resize(sz.x, sz.y); }
	};
}
