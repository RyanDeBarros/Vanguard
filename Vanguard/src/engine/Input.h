#pragma once

#include "Vendor.h"
#include "Events.h"

namespace vg
{
	class Window;

	namespace _
	{
		extern void assign_window_callbacks(Window& window);
		extern void init_input();
	}

	namespace input
	{
		enum class Key
		{
			A = GLFW_KEY_A,
			B = GLFW_KEY_B,
			C = GLFW_KEY_C,
			D = GLFW_KEY_D,
			E = GLFW_KEY_E,
			F = GLFW_KEY_F,
			G = GLFW_KEY_G,
			H = GLFW_KEY_H,
			I = GLFW_KEY_I,
			J = GLFW_KEY_J,
			K = GLFW_KEY_K,
			L = GLFW_KEY_L,
			M = GLFW_KEY_M,
			N = GLFW_KEY_N,
			O = GLFW_KEY_O,
			P = GLFW_KEY_P,
			Q = GLFW_KEY_Q,
			R = GLFW_KEY_R,
			S = GLFW_KEY_S,
			T = GLFW_KEY_T,
			U = GLFW_KEY_U,
			V = GLFW_KEY_V,
			W = GLFW_KEY_W,
			X = GLFW_KEY_X,
			Y = GLFW_KEY_Y,
			Z = GLFW_KEY_Z,
			SPACE = GLFW_KEY_SPACE,
			ROW_0 = GLFW_KEY_0,
			ROW_1 = GLFW_KEY_1,
			ROW_2 = GLFW_KEY_2,
			ROW_3 = GLFW_KEY_3,
			ROW_4 = GLFW_KEY_4,
			ROW_5 = GLFW_KEY_5,
			ROW_6 = GLFW_KEY_6,
			ROW_7 = GLFW_KEY_7,
			ROW_8 = GLFW_KEY_8,
			ROW_9 = GLFW_KEY_9,
			NP_0 = GLFW_KEY_KP_0,
			NP_1 = GLFW_KEY_KP_1,
			NP_2 = GLFW_KEY_KP_2,
			NP_3 = GLFW_KEY_KP_3,
			NP_4 = GLFW_KEY_KP_4,
			NP_5 = GLFW_KEY_KP_5,
			NP_6 = GLFW_KEY_KP_6,
			NP_7 = GLFW_KEY_KP_7,
			NP_8 = GLFW_KEY_KP_8,
			NP_9 = GLFW_KEY_KP_9,
			CONTROL_LEFT = GLFW_KEY_LEFT_CONTROL,
			CONTROL_RIGHT = GLFW_KEY_RIGHT_CONTROL,
			ALT_LEFT = GLFW_KEY_LEFT_ALT,
			ALT_RIGHT = GLFW_KEY_RIGHT_ALT,
			SHIFT_LEFT = GLFW_KEY_LEFT_SHIFT,
			SHIFT_RIGHT = GLFW_KEY_RIGHT_SHIFT,
			SUPER_LEFT = GLFW_KEY_LEFT_SUPER,
			SUPER_RIGHT = GLFW_KEY_RIGHT_SUPER,
			BRACKET_LEFT = GLFW_KEY_LEFT_BRACKET,
			BRACKET_RIGHT = GLFW_KEY_RIGHT_BRACKET,
			ENTER = GLFW_KEY_ENTER,
			TAB = GLFW_KEY_TAB,
			CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
			NUM_LOCK = GLFW_KEY_NUM_LOCK,
			PERIOD = GLFW_KEY_PERIOD,
			COMMA = GLFW_KEY_COMMA,
			APOSTROPHE = GLFW_KEY_APOSTROPHE,
			SEMICOLON = GLFW_KEY_SEMICOLON,
			SLASH = GLFW_KEY_SLASH,
			BACKSLASH = GLFW_KEY_BACKSLASH,
			BACKTICK = GLFW_KEY_GRAVE_ACCENT,
			BACKSPACE = GLFW_KEY_BACKSPACE,
			ESCAPE = GLFW_KEY_ESCAPE,
			DELETE = GLFW_KEY_DELETE,
			INSERT = GLFW_KEY_INSERT,
			HOME = GLFW_KEY_HOME,
			END = GLFW_KEY_END,
			PAGE_UP = GLFW_KEY_PAGE_UP,
			PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
			SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
			PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
			PAUSE = GLFW_KEY_PAUSE,
			MENU = GLFW_KEY_MENU,
			F1 = GLFW_KEY_F1,
			F2 = GLFW_KEY_F2,
			F3 = GLFW_KEY_F3,
			F4 = GLFW_KEY_F4,
			F5 = GLFW_KEY_F5,
			F6 = GLFW_KEY_F6,
			F7 = GLFW_KEY_F7,
			F8 = GLFW_KEY_F8,
			F9 = GLFW_KEY_F9,
			F10 = GLFW_KEY_F10,
			F11 = GLFW_KEY_F11,
			F12 = GLFW_KEY_F12,
			F13 = GLFW_KEY_F13,
			F14 = GLFW_KEY_F14,
			F15 = GLFW_KEY_F15,
			F16 = GLFW_KEY_F16,
			F17 = GLFW_KEY_F17,
			F18 = GLFW_KEY_F18,
			F19 = GLFW_KEY_F19,
			F20 = GLFW_KEY_F20,
			F21 = GLFW_KEY_F21,
			F22 = GLFW_KEY_F22,
			F23 = GLFW_KEY_F23,
			F24 = GLFW_KEY_F24,
			F25 = GLFW_KEY_F25,
			MINUS = GLFW_KEY_MINUS,
			EQUAL = GLFW_KEY_EQUAL,
			NP_PLUS = GLFW_KEY_KP_ADD,
			NP_MINUS = GLFW_KEY_KP_SUBTRACT,
			NP_PERIOD = GLFW_KEY_KP_DECIMAL,
			NP_STAR = GLFW_KEY_KP_MULTIPLY,
			NP_SLASH = GLFW_KEY_KP_DIVIDE,
			NP_ENTER = GLFW_KEY_KP_ENTER,
			NP_EQUAL = GLFW_KEY_KP_EQUAL,
			LEFT = GLFW_KEY_LEFT,
			UP = GLFW_KEY_UP,
			RIGHT = GLFW_KEY_RIGHT,
			DOWN = GLFW_KEY_DOWN,
			UNKNOWN = GLFW_KEY_UNKNOWN,
			WORLD_1 = GLFW_KEY_WORLD_1,
			WORLD_2 = GLFW_KEY_WORLD_2,
		};

		extern const char* get_key_name(Key key, int scancode = 0);
		extern int get_scancode(Key key);

		namespace Mods
		{
			enum
			{
				SHIFT = GLFW_MOD_SHIFT,
				CONTROL = GLFW_MOD_CONTROL,
				ALT = GLFW_MOD_ALT,
				SUPER = GLFW_MOD_SUPER,
				CAPS_LOCK = GLFW_MOD_CAPS_LOCK,
				NUM_LOCK = GLFW_MOD_NUM_LOCK
			};
		}

		enum class MouseButton
		{
			LEFT = GLFW_MOUSE_BUTTON_LEFT,
			RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
			MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
			EXTRA_1 = GLFW_MOUSE_BUTTON_4,
			EXTRA_2 = GLFW_MOUSE_BUTTON_5,
			EXTRA_3 = GLFW_MOUSE_BUTTON_6,
			EXTRA_4 = GLFW_MOUSE_BUTTON_7,
			EXTRA_5 = GLFW_MOUSE_BUTTON_8
		};

		enum class Action
		{
			PRESS = GLFW_PRESS,
			RELEASE = GLFW_RELEASE,
			DOWN = GLFW_REPEAT
		};

		enum class Controller
		{
			J1 = GLFW_JOYSTICK_1,
			J2 = GLFW_JOYSTICK_2,
			J3 = GLFW_JOYSTICK_3,
			J4 = GLFW_JOYSTICK_4,
			J5 = GLFW_JOYSTICK_5,
			J6 = GLFW_JOYSTICK_6,
			J7 = GLFW_JOYSTICK_7,
			J8 = GLFW_JOYSTICK_8,
			J9 = GLFW_JOYSTICK_9,
			J10 = GLFW_JOYSTICK_10,
			J11 = GLFW_JOYSTICK_11,
			J12 = GLFW_JOYSTICK_12,
			J13 = GLFW_JOYSTICK_13,
			J14 = GLFW_JOYSTICK_14,
			J15 = GLFW_JOYSTICK_15,
			J16 = GLFW_JOYSTICK_16
		};

		enum class GamepadButton
		{
			FACE_UP = GLFW_GAMEPAD_BUTTON_TRIANGLE,
			FACE_RIGHT = GLFW_GAMEPAD_BUTTON_CIRCLE,
			FACE_DOWN = GLFW_GAMEPAD_BUTTON_CROSS,
			FACE_LEFT = GLFW_GAMEPAD_BUTTON_SQUARE,
			DPAD_UP = GLFW_GAMEPAD_BUTTON_DPAD_UP,
			DPAD_RIGHT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
			DPAD_DOWN = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
			DPAD_LEFT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
			START = GLFW_GAMEPAD_BUTTON_START,
			SELECT = GLFW_GAMEPAD_BUTTON_GUIDE,
			BACK = GLFW_GAMEPAD_BUTTON_BACK,
			LEFT_BUMPER = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
			RIGHT_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
			LEFT_STICK = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
			RIGHT_STICK = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB
		};

		enum class GamepadAxis
		{
			LEFT_X = GLFW_GAMEPAD_AXIS_LEFT_X,
			LEFT_Y = GLFW_GAMEPAD_AXIS_LEFT_Y,
			RIGHT_X = GLFW_GAMEPAD_AXIS_RIGHT_X,
			RIGHT_Y = GLFW_GAMEPAD_AXIS_RIGHT_Y,
			LEFT_TRIGGER = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
			RIGHT_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
		};

		enum class DPadDirection
		{
			UP = GLFW_HAT_UP,
			RIGHT = GLFW_HAT_RIGHT,
			DOWN = GLFW_HAT_DOWN,
			LEFT = GLFW_HAT_LEFT,
			RIGHT_UP = GLFW_HAT_RIGHT_UP,
			LEFT_UP = GLFW_HAT_LEFT_UP,
			RIGHT_DOWN = GLFW_HAT_RIGHT_DOWN,
			LEFT_DOWN = GLFW_HAT_LEFT_DOWN,
			CENTER = GLFW_HAT_CENTERED,
		};

		class Gamepad
		{
			GLFWgamepadstate _g;
			Controller _c;
		
		public:
			Gamepad(Controller c);

			Controller controller() const;
			bool is_connected() const;
			bool has_mapping() const;
			Action button_state(GamepadButton button) const;
			float axis_state(GamepadAxis axis) const;
			int dpad_state(unsigned int dpad = 0) const;
			const char* name() const;
			const char* identifier() const;
		};
		
		extern bool update_gamepad_mapping(const char* sdl_game_controller_db);
		extern void set_joystick_connection_callback(void(*callback)(Controller, bool));
		extern void(*get_joystick_connection_callback())(Controller, bool);

		struct CharEvent : public Event
		{
			Key key;
		};
		typedef EventHandler<CharEvent> CharEventHandler;

		struct CharModsEvent : public Event
		{
			Key key;
			int mods;
		};
		typedef EventHandler<CharModsEvent> CharModsEventHandler;
		
		struct CursorEnterEvent : public Event
		{
			bool entered;
		};
		typedef EventHandler<CursorEnterEvent> CursorEnterEventHandler;

		struct CursorPosEvent : public Event
		{
			double x, y;
		};
		typedef EventHandler<CursorPosEvent> CursorPosEventHandler;

		struct PathDropEvent : public Event
		{
			int count;
			const char** paths;
		};
		typedef EventHandler<PathDropEvent> PathDropEventHandler;

		struct FramebufferResizeEvent : public Event
		{
			int w, h;
		};
		typedef EventHandler<FramebufferResizeEvent> FramebufferResizeEventHandler;

		struct KeyEvent : public Event
		{
			Key key;
			int scancode;
			Action action;
			int mods;
		};
		typedef EventHandler<KeyEvent> KeyEventHandler;

		struct MouseButtonEvent : public Event
		{
			MouseButton button;
			Action action;
			int mods;
		};
		typedef EventHandler<MouseButtonEvent> MouseButtonEventHandler;

		struct ScrollEvent : public Event
		{
			double x, y;
		};
		typedef EventHandler<ScrollEvent> ScrollEventHandler;

		struct WindowCloseEvent : public Event
		{
		};
		typedef EventHandler<WindowCloseEvent> WindowCloseEventHandler;

		struct WindowContentScaleEvent : public Event
		{
			float x, y;
		};
		typedef EventHandler<WindowContentScaleEvent> WindowContentScaleEventHandler;

		struct WindowFocusEvent : public Event
		{
			bool focused;
		};
		typedef EventHandler<WindowFocusEvent> WindowFocusEventHandler;

		struct WindowIconifyEvent : public Event
		{
			bool iconified;
		};
		typedef EventHandler<WindowIconifyEvent> WindowIconifyEventHandler;

		struct WindowMaximizeEvent : public Event
		{
			bool maximized;
		};
		typedef EventHandler<WindowMaximizeEvent> WindowMaximizeEventHandler;

		struct WindowPosEvent : public Event
		{
			int x, y;
		};
		typedef EventHandler<WindowPosEvent> WindowPosEventHandler;
		
		struct WindowRefreshEvent : public Event
		{
		};
		typedef EventHandler<WindowRefreshEvent> WindowRefreshEventHandler;

		struct WindowResizeEvent : public Event
		{
			int w, h;
		};
		typedef EventHandler<WindowResizeEvent> WindowResizeEventHandler;
	}
}
