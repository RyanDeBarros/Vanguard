#include <iostream>
#include <string>

#include "Vanguard.h"

#include "raii/Window.h"

int main()
{
	std::cout << "Welcome to Vanguard!" << std::endl;
	vg::init();
	
	vg::Window window(1440, 1080, "Hello World");

	vg::set_clear_color({ 0.2f, 0.3f, 0.8f, 1.0f });

	window.root_input_handlers.character.callback = [](vg::input::CharEvent c) { std::cout << std::to_string((int)c.key) << std::endl; c.consumed = true; };

	for (;;)
	{
		glfwPollEvents();
		if (window.should_close())
			break;

		window.new_frame();

		// frame

		window.end_frame();
	}

	vg::terminate();
	return 0;
}
